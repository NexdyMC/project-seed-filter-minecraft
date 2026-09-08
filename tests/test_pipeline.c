#include <string.h>
#include "seedfilter.h"
#include "biomes.h"  /* MC_1_16_1 */
#include <stdio.h>
#include <assert.h>
#include <stdatomic.h>

static atomic_long g_progressCount = 0;
static atomic_long g_resultCbCount = 0;

static void on_progress(int64_t seed, void *ud) {
    (void)seed; (void)ud;
    atomic_fetch_add(&g_progressCount, 1);
}

static void on_result(const SFSeedResult *r, void *ud) {
    (void)ud;
    atomic_fetch_add(&g_resultCbCount, 1);
    printf("[result_cb] seed=%lld hitCount=%d\n", (long long)r->seed, r->hitCount);
}

int main(void) {
    SFStructureType targets[] = { SF_VILLAGE, SF_TREASURE, SF_SHIPWRECK };

    SFSearchParams params;
    params.mcVersion = MC_1_16_1;
    params.playerX = 0;
    params.playerZ = 0;
    params.searchRadius = 1024;
    params.radiusPlayer = 32;
    params.targetStructures = targets;
    params.targetStructureCount = 3;
    params.requireAll = 0;              /* minimal 1 struktur lolos */
    params.minDigits = 1;
    params.maxDigits = 6;                /* seed kecil supaya cepat ketemu utk test */
    params.allowNegative = 1;
    params.maxAttempts = 3000;
    params.maxResults = 5;
    params.threadCount = 4;              /* uji multi-thread */
    params.rngInitState = 42;            /* deterministik untuk test */

    SFSearchOutput output;
    int rc = run_seed_search(&params, &output, on_progress, on_result, NULL);
    assert(rc == 0);

    printf("rc=%d attemptsMade=%lld resultCount=%d\n",
           rc, (long long)output.attemptsMade, output.resultCount);
    printf("progress_cb dipanggil %ld kali, result_cb dipanggil %ld kali\n",
           atomic_load(&g_progressCount), atomic_load(&g_resultCbCount));

    assert(output.attemptsMade > 0);
    assert((long)atomic_load(&g_progressCount) == output.attemptsMade);
    assert((long)atomic_load(&g_resultCbCount) == output.resultCount);
    assert(output.resultCount > 0 && "harus nemu minimal 1 seed valid dalam 3000 attempt radius 1024");

    /* cek isi salah satu hasil + serialisasi JSON */
    char *json = search_output_to_json(&output);
    assert(json != NULL);
    printf("JSON (%zu bytes): %.300s...\n", strlen(json), json);
    sf_free_json(json);

    free_search_output(&output);
    assert(output.results == NULL && output.resultCount == 0);

    printf("SEMUA TEST PIPELINE (run_seed_search) LULUS\n");
    return 0;
}
