#include "finder.h"
#include "validate.h"
#include "finders.h"     /* cubiomes: StructureType, getStructurePos, dst */

#include <stdlib.h>
#include <string.h>

/* ---- SFHitList: array dinamis sederhana ----------------------------- */

void sf_hitlist_init(SFHitList *list) {
    list->hits = NULL;
    list->count = 0;
    list->capacity = 0;
}

void sf_hitlist_push(SFHitList *list, SFStructureHit hit) {
    if (list->count == list->capacity) {
        int newCap = list->capacity == 0 ? 8 : list->capacity * 2;
        SFStructureHit *nh = (SFStructureHit *)realloc(list->hits, (size_t)newCap * sizeof(SFStructureHit));
        if (!nh) return; /* alokasi gagal: hit ini dibuang, tidak crash */
        list->hits = nh;
        list->capacity = newCap;
    }
    list->hits[list->count++] = hit;
}

void sf_hitlist_free(SFHitList *list) {
    free(list->hits);
    list->hits = NULL;
    list->count = 0;
    list->capacity = 0;
}

/* ---- Pemetaan SFStructureType -> enum StructureType milik cubiomes -- */

static int sf_to_cubiomes_type(SFStructureType t) {
    switch (t) {
        case SF_TREASURE:       return Treasure;
        case SF_VILLAGE:        return Village;
        case SF_SHIPWRECK:      return Shipwreck;
        case SF_RUINED_PORTAL:  return Ruined_Portal;
        case SF_MANSION:        return Mansion;
        case SF_BASTION:        return Bastion;
        case SF_FORTRESS:       return Fortress;
        case SF_END_CITY:       return End_City;
        case SF_DESERT_PYRAMID: return Desert_Pyramid;
        case SF_JUNGLE_TEMPLE:  return Jungle_Temple;
        case SF_MINESHAFT:      return Mineshaft;
        case SF_IGLOO:          return Igloo;
        case SF_MONUMENT:       return Monument;
        default:                return -1;
    }
}

/* Pembagian bulat ke bawah (floor), beda dengan '/' bawaan C yang
 * membulatkan ke arah nol -- penting untuk region coordinate negatif. */
static int floorDivInt(int a, int b) {
    int d = a / b;
    int r = a % b;
    if (r != 0 && ((r < 0) != (b < 0))) d--;
    return d;
}

/* Cari satu tipe struktur di sekitar player, tambahkan semua hit
 * (lolos atau tidak) ke outList. Mengembalikan jumlah yang lolos. */
static int find_one_structure_type(Generator *g, int64_t seed, int mcVersion,
                                    int playerX, int playerZ,
                                    int searchRadius, int radiusPlayer,
                                    SFStructureType sfType,
                                    SFHitList *outList) {
    int cbType = sf_to_cubiomes_type(sfType);
    if (cbType < 0) return 0;

    StructureConfig sconf;
    if (!getStructureConfig(cbType, mcVersion, &sconf)) {
        return 0; /* struktur ini tidak berlaku di versi MC yang dipilih */
    }

    int regionBlocks = (int)sconf.regionSize * 16;
    if (regionBlocks <= 0) return 0;

    int regionXMin = floorDivInt(playerX - searchRadius, regionBlocks);
    int regionXMax = floorDivInt(playerX + searchRadius, regionBlocks);
    int regionZMin = floorDivInt(playerZ - searchRadius, regionBlocks);
    int regionZMax = floorDivInt(playerZ + searchRadius, regionBlocks);

    int radiusStruct = sf_struct_radius(sfType);
    int passedCount = 0;

    uint64_t useed = (uint64_t)seed;

    /* dim: 0=overworld, -1=nether, 1=end (lihat DIM_* di generator.h);
     * cubiomes memberi tahu lewat sconf.dim struktur ini "milik" dimensi
     * mana, jadi kita ikuti itu alih-alih menebak sendiri. */
    applySeed(g, sconf.dim, useed);

    for (int rx = regionXMin; rx <= regionXMax; rx++) {
        for (int rz = regionZMin; rz <= regionZMax; rz++) {
            Pos pos;
            if (!getStructurePos(cbType, mcVersion, useed, rx, rz, &pos)) {
                continue;
            }
            if (!isViableStructurePos(cbType, g, pos.x, pos.z, 0)) {
                continue;
            }

            int dx, dz, threshold;
            int ok = sf_validate_distance(playerX, playerZ, pos.x, pos.z,
                                           radiusPlayer, radiusStruct,
                                           &dx, &dz, &threshold);

            SFStructureHit hit;
            hit.type = sfType;
            hit.x = pos.x;
            hit.z = pos.z;
            hit.dx = dx;
            hit.dz = dz;
            hit.distManhattan = dx + dz;
            hit.radiusThreshold = threshold;
            hit.passed = ok;

            sf_hitlist_push(outList, hit);
            if (ok) passedCount++;
        }
    }

    return passedCount;
}

int sf_find_structures(Generator *g, int64_t seed, int mcVersion,
                        int playerX, int playerZ,
                        int searchRadius, int radiusPlayer,
                        const SFStructureType *targets, int targetCount,
                        SFHitList *outList) {
    int totalPassed = 0;
    for (int i = 0; i < targetCount; i++) {
        totalPassed += find_one_structure_type(g, seed, mcVersion,
                                                playerX, playerZ,
                                                searchRadius, radiusPlayer,
                                                targets[i], outList);
    }
    return totalPassed;
}
