#ifndef SF_FINDER_H
#define SF_FINDER_H

#include <stdint.h>
#include "seedfilter.h"
#include "generator.h"   /* dari cubiomes: struct Generator */

#ifdef __cplusplus
extern "C" {
#endif

/* List dinamis SFStructureHit (dipakai internal, dibungkus jadi array
 * biasa di seedfilter.c sebelum diserahkan ke caller). */
typedef struct {
    SFStructureHit *hits;
    int count;
    int capacity;
} SFHitList;

void sf_hitlist_init(SFHitList *list);
void sf_hitlist_push(SFHitList *list, SFStructureHit hit);
void sf_hitlist_free(SFHitList *list);

/* Cari semua struktur target di sekitar (playerX, playerZ) dalam radius
 * searchRadius (block), untuk seed & mcVersion tertentu, lalu validasi
 * jaraknya terhadap radiusPlayer. 'g' adalah Generator milik CALLER
 * (biasanya satu per thread, dibuat sekali & dipakai ulang lintas seed
 * lewat setupGenerator() di luar fungsi ini) -- fungsi ini hanya
 * memanggil applySeed() di dalamnya.
 *
 * Mengembalikan jumlah hit yang LOLOS validasi (bisa 0). Semua hit
 * (lolos maupun tidak) tetap dimasukkan ke outList supaya GUI bisa
 * menampilkan status lolos/tidak per struktur. */
int sf_find_structures(Generator *g, int64_t seed, int mcVersion,
                        int playerX, int playerZ,
                        int searchRadius, int radiusPlayer,
                        const SFStructureType *targets, int targetCount,
                        SFHitList *outList);

#ifdef __cplusplus
}
#endif

#endif /* SF_FINDER_H */
