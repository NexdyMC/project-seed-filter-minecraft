#include "finder.h"
#include "seedfilter.h"
#include "generator.h"
#include "biomes.h"   /* MC_1_16_1 */
#include <stdio.h>
#include <assert.h>

int main(void) {
    Generator g;
    setupGenerator(&g, MC_1_16_1, 0);

    SFStructureType targets[] = { SF_VILLAGE, SF_TREASURE };
    int found = 0;

    /* Cari beberapa seed pertama yang punya Village dalam radius 512 block
     * dari (0,0) -- ini murni untuk verifikasi bahwa wrapper Cubiomes kita
     * jalan dan tidak crash / tidak salah hitung. */
    for (int64_t seed = 0; seed < 200 && found < 3; seed++) {
        SFHitList list;
        sf_hitlist_init(&list);

        int passed = sf_find_structures(&g, seed, MC_1_16_1, 0, 0, 512, 16,
                                         targets, 2, &list);

        if (passed > 0) {
            found++;
            printf("seed=%lld -> %d hit terdaftar, %d lolos\n",
                   (long long)seed, list.count, passed);
            for (int i = 0; i < list.count; i++) {
                SFStructureHit *h = &list.hits[i];
                printf("   type=%d pos=(%d,%d) dx=%d dz=%d th=%d passed=%d\n",
                       h->type, h->x, h->z, h->dx, h->dz, h->radiusThreshold, h->passed);
            }
        }
        sf_hitlist_free(&list);
    }

    printf("total seed dengan hit ditemukan dalam 200 percobaan: %d\n", found);
    assert(found > 0 && "harus ada minimal 1 seed dengan struktur dekat dalam 200 percobaan");
    printf("SEMUA TEST FINDER LULUS\n");
    return 0;
}
