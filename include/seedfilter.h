#ifndef SEEDFILTER_H
#define SEEDFILTER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Export/import macro untuk DLL Windows ------------------------- */
#if defined(_WIN32) && defined(SEEDFILTER_BUILD_DLL)
  #define SF_API __declspec(dllexport)
#elif defined(_WIN32) && defined(SEEDFILTER_USE_DLL)
  #define SF_API __declspec(dllimport)
#else
  #define SF_API
#endif

/* ---- Tipe struktur yang didukung ------------------------------------ */
typedef enum {
    SF_TREASURE = 0,
    SF_VILLAGE,
    SF_SHIPWRECK,
    SF_RUINED_PORTAL,
    SF_MANSION,
    SF_BASTION,
    SF_FORTRESS,
    SF_END_CITY,
    SF_DESERT_PYRAMID,   /* "desert" pada spesifikasi = Desert Pyramid   */
    SF_JUNGLE_TEMPLE,    /* "jungle" pada spesifikasi = Jungle Temple    */
    SF_MINESHAFT,
    SF_IGLOO,
    SF_MONUMENT,
    SF_STRUCT_COUNT
} SFStructureType;

/* ---- Tipe biome yang didukung (untuk pengecekan biome di posisi player) */
typedef enum {
    SF_BIOME_OCEAN = 0,
    SF_BIOME_DEEP_OCEAN,
    SF_BIOME_BEACH,
    SF_BIOME_PLAINS,
    SF_BIOME_DESERT,
    SF_BIOME_COUNT
} SFBiomeType;

/* Satu struktur yang ditemukan + hasil validasi jaraknya */
typedef struct {
    SFStructureType type;
    int x, z;
    int dx, dz;            /* dx = |x - playerX|, dz = |z - playerZ|     */
    int distManhattan;     /* dx + dz                                    */
    int radiusThreshold;   /* radiusPlayer + radiusStruktur yang dipakai */
    int passed;            /* 1 kalau lolos ambang batas jarak           */
} SFStructureHit;

/* Hasil evaluasi satu seed */
typedef struct {
    int64_t seed;
    SFStructureHit *hits;  /* dialokasikan oleh library                  */
    int hitCount;
    int passed;            /* 1 kalau seed ini dinyatakan valid          */
} SFSeedResult;

/* Parameter pencarian */
typedef struct {
    int mcVersion;                        /* pakai MC_1_16_1 dari cubiomes */
    int playerX, playerZ;                 /* posisi player/spawn acuan     */
    int searchRadius;                     /* radius pencarian region (block)*/
    int radiusPlayer;                     /* radius toleransi player        */
    const SFStructureType *targetStructures;
    int targetStructureCount;
    int requireAll;                       /* 1=semua target harus lolos, 0=minimal satu */
    int minDigits, maxDigits;             /* panjang digit seed acak (1..19) */
    int allowNegative;                    /* izinkan seed negatif            */
    int64_t maxAttempts;                  /* 0 = tak terbatas s.d. maxResults */
    int maxResults;                       /* 0 = tak terbatas s.d. maxAttempts*/
    int threadCount;                      /* 0/1 = single-thread             */
    uint64_t rngInitState;                /* 0 = auto (seed dari waktu)       */
} SFSearchParams;

/* Dipanggil dari thread worker setiap kali sebuah seed baru dievaluasi.
 * Boleh dipanggil dari banyak thread sekaligus -> implementasi callback
 * di sisi pemanggil (GUI) HARUS thread-safe sendiri (mis. lewat queue). */
typedef void (*SFProgressCallback)(int64_t current_seed, void *user_data);

/* Dipanggil setiap kali seed valid ditemukan (opsional, boleh NULL).
 * 'result' hanya valid selama callback berjalan; jika ingin disimpan,
 * salin datanya -- jangan simpan pointer-nya. */
typedef void (*SFResultCallback)(const SFSeedResult *result, void *user_data);

typedef struct {
    SFSeedResult *results;   /* dialokasikan oleh library                */
    int resultCount;
    int64_t attemptsMade;
} SFSearchOutput;

/* Jalankan seluruh pipeline generate -> find -> validate -> output.
 * Return 0 = sukses, non-zero = kode error (lihat seedfilter_errors.h
 * kalau dibutuhkan detail; untuk versi ini: -1 = parameter tidak valid).
 *
 * Kepemilikan memori:
 *  - 'output' diisi oleh library; panggil free_search_output(output)
 *    setelah selesai memakainya, JANGAN panggil free() manual pada
 *    output->results atau output->results[i].hits.
 *  - callback TIDAK boleh menyimpan pointer ke dalam SFSeedResult,
 *    karena memori itu milik 'output' dan akan di-free bersamaan. */
SF_API int run_seed_search(const SFSearchParams *params,
                            SFSearchOutput *output,
                            SFProgressCallback progress_cb,
                            SFResultCallback result_cb,
                            void *user_data);

/* Membebaskan seluruh memori di dalam SFSearchOutput (termasuk semua
 * hits di setiap SFSeedResult). Aman dipanggil walau output kosong. */
SF_API void free_search_output(SFSearchOutput *output);

/* Serialisasi satu SFSeedResult menjadi JSON. String yang dikembalikan
 * dialokasikan oleh library (malloc di dalam .so/.dll ini) -- WAJIB
 * dibebaskan lewat sf_free_json(), bukan free() biasa, supaya alokasi
 * dan dealokasi terjadi di allocator/heap yang sama (penting terutama
 * di Windows DLL dengan CRT yang berbeda). */
SF_API char *seed_result_to_json(const SFSeedResult *result);
SF_API void sf_free_json(char *json);

/* Serialisasi seluruh SFSearchOutput menjadi satu array JSON. */
SF_API char *search_output_to_json(const SFSearchOutput *output);

#ifdef __cplusplus
}
#endif

#endif /* SEEDFILTER_H */
