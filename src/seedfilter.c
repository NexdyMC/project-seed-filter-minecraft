#include "seedfilter.h"
#include "seed.h"
#include "finder.h"
#include "validate.h"
#include "generator.h"   /* cubiomes: Generator, setupGenerator */

#include <pthread.h>
#include <stdlib.h>
#include <string.h>

/* ---- State bersama antar-thread -------------------------------------- */
typedef struct {
    pthread_mutex_t mutex;

    SFSeedResult *results;
    int resultCount;
    int resultCapacity;

    int64_t attempts;
    int stop;

    const SFSearchParams *params;
    SFProgressCallback progress_cb;
    SFResultCallback result_cb;
    void *user_data;

    SFRng baseRng; /* HANYA diakses di bawah mutex (dipakai untuk
                      menurunkan RNG anak per-thread) */
} SFSearchShared;

typedef struct {
    SFSearchShared *shared;
    int threadIndex;
} SFWorkerArg;

/* Cek & update kondisi berhenti (habis attempt/result) secara atomik
 * lewat mutex. Mengembalikan 1 kalau worker HARUS berhenti. */
static int shared_should_stop_and_reserve_attempt(SFSearchShared *sh) {
    int stop;
    pthread_mutex_lock(&sh->mutex);
    if (sh->stop) {
        stop = 1;
    } else if (sh->params->maxAttempts > 0 && sh->attempts >= sh->params->maxAttempts) {
        sh->stop = 1;
        stop = 1;
    } else if (sh->params->maxResults > 0 && sh->resultCount >= sh->params->maxResults) {
        sh->stop = 1;
        stop = 1;
    } else {
        sh->attempts++;
        stop = 0;
    }
    pthread_mutex_unlock(&sh->mutex);
    return stop;
}

/* Pindahkan kepemilikan sebuah SFSeedResult (hasil valid) ke array
 * hasil bersama. Dipanggil dengan mutex TERKUNCI oleh caller. */
static void shared_push_result_locked(SFSearchShared *sh, SFSeedResult result) {
    if (sh->resultCount == sh->resultCapacity) {
        int newCap = sh->resultCapacity == 0 ? 8 : sh->resultCapacity * 2;
        SFSeedResult *nr = (SFSeedResult *)realloc(sh->results, (size_t)newCap * sizeof(SFSeedResult));
        if (nr) {
            sh->results = nr;
            sh->resultCapacity = newCap;
        }
    }
    if (sh->resultCount < sh->resultCapacity) {
        sh->results[sh->resultCount++] = result;
    } else {
        /* realloc gagal (OOM) -- buang hasil ini supaya tidak memory leak,
         * daripada menyimpan pointer yatim yang tak tercatat di 'output'. */
        free(result.hits);
    }
}

static void *worker_main(void *argPtr) {
    SFWorkerArg *wa = (SFWorkerArg *)argPtr;
    SFSearchShared *sh = wa->shared;
    const SFSearchParams *p = sh->params;

    /* Satu Generator per thread, dipakai ulang untuk semua seed yang
     * dievaluasi thread ini (setupGenerator relatif mahal, applySeed
     * yang murah dipanggil per-seed di dalam sf_find_structures). */
    Generator g;
    setupGenerator(&g, p->mcVersion, 0);

    SFRng rng;
    pthread_mutex_lock(&sh->mutex);
    sf_rng_spawn_child(&sh->baseRng, &rng, wa->threadIndex);
    pthread_mutex_unlock(&sh->mutex);

    while (!shared_should_stop_and_reserve_attempt(sh)) {
        int64_t seed = sf_generate_seed(&rng, p->minDigits, p->maxDigits, p->allowNegative);

        /* Callback progress dipanggil untuk SETIAP seed yang dievaluasi,
         * dari thread manapun -- sisi pemanggil (GUI) bertanggung jawab
         * membuat callback ini thread-safe (mis. lewat queue/mutex sendiri). */
        if (sh->progress_cb) {
            sh->progress_cb(seed, sh->user_data);
        }

        SFHitList list;
        sf_hitlist_init(&list);

        int passedCount = sf_find_structures(&g, seed, p->mcVersion,
                                              p->playerX, p->playerZ,
                                              p->searchRadius, p->radiusPlayer,
                                              p->targetStructures, p->targetStructureCount,
                                              &list);

        int seedPassed;
        if (p->requireAll) {
            seedPassed = (p->targetStructureCount > 0) && (passedCount == p->targetStructureCount);
        } else {
            seedPassed = (passedCount > 0);
        }

        if (seedPassed) {
            SFSeedResult result;
            result.seed = seed;
            result.hitCount = list.count;
            result.passed = 1;
            /* Pindahkan (bukan salin) buffer hits: kepemilikan memori
             * berpindah dari 'list' ke 'result'. Karena itu list di-reset
             * manual di sini, BUKAN lewat sf_hitlist_free (yang akan
             * membebaskan buffer yang baru saja kita pindahkan). */
            result.hits = list.hits;
            list.hits = NULL;
            list.count = 0;
            list.capacity = 0;

            if (sh->result_cb) {
                sh->result_cb(&result, sh->user_data);
            }

            pthread_mutex_lock(&sh->mutex);
            shared_push_result_locked(sh, result);
            pthread_mutex_unlock(&sh->mutex);
        } else {
            sf_hitlist_free(&list);
        }
    }

    return NULL;
}

static int validate_params(const SFSearchParams *p) {
    if (!p) return -1;
    if (p->targetStructures == NULL || p->targetStructureCount <= 0) return -1;
    if (p->minDigits < 1 || p->maxDigits > 19 || p->minDigits > p->maxDigits) return -1;
    if (p->searchRadius <= 0) return -1;
    return 0;
}

int run_seed_search(const SFSearchParams *params,
                     SFSearchOutput *output,
                     SFProgressCallback progress_cb,
                     SFResultCallback result_cb,
                     void *user_data) {
    if (!output) return -1;
    memset(output, 0, sizeof(*output));

    if (validate_params(params) != 0) return -1;

    int threadCount = params->threadCount;
    if (threadCount < 1) threadCount = 1;

    SFSearchShared shared;
    memset(&shared, 0, sizeof(shared));
    if (pthread_mutex_init(&shared.mutex, NULL) != 0) return -2;

    shared.params = params;
    shared.progress_cb = progress_cb;
    shared.result_cb = result_cb;
    shared.user_data = user_data;
    sf_rng_init(&shared.baseRng, params->rngInitState);

    pthread_t *threads = (pthread_t *)malloc((size_t)threadCount * sizeof(pthread_t));
    SFWorkerArg *args = (SFWorkerArg *)malloc((size_t)threadCount * sizeof(SFWorkerArg));
    if (!threads || !args) {
        free(threads);
        free(args);
        pthread_mutex_destroy(&shared.mutex);
        return -3;
    }

    for (int i = 0; i < threadCount; i++) {
        args[i].shared = &shared;
        args[i].threadIndex = i;
        if (pthread_create(&threads[i], NULL, worker_main, &args[i]) != 0) {
            /* gagal spawn thread ke-i: tandai stop supaya thread yang
             * sudah jalan tidak berjalan selamanya, lalu tunggu semua
             * yang berhasil dibuat sebelum keluar */
            pthread_mutex_lock(&shared.mutex);
            shared.stop = 1;
            pthread_mutex_unlock(&shared.mutex);
            for (int j = 0; j < i; j++) pthread_join(threads[j], NULL);
            free(threads);
            free(args);
            pthread_mutex_destroy(&shared.mutex);
            free_search_output(output); /* jaga-jaga kalau ada hasil parsial */
            return -4;
        }
    }

    for (int i = 0; i < threadCount; i++) {
        pthread_join(threads[i], NULL);
    }

    output->results = shared.results;
    output->resultCount = shared.resultCount;
    output->attemptsMade = shared.attempts;

    free(threads);
    free(args);
    pthread_mutex_destroy(&shared.mutex);
    return 0;
}

void free_search_output(SFSearchOutput *output) {
    if (!output) return;
    for (int i = 0; i < output->resultCount; i++) {
        free(output->results[i].hits);
    }
    free(output->results);
    output->results = NULL;
    output->resultCount = 0;
    output->attemptsMade = 0;
}
