// gcc main001.c cubiomes\libcubiomes.a -o structure 
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "cubiomes/generator.h"
#include "cubiomes/biomes.h"
#include "cubiomes/finders.h"

// | ------- | ---- | -------- | ------------------ | 
// |   Shift |   2ⁿ |   Dibagi | Umum dipakai untuk |
// | ------- | ---- | -------- | ------------------ | 
// |  `>> 1` |    2 |    `/ 2` | setengah nilai     |
// |  `>> 2` |    4 |    `/ 4` | kuadran            |
// |  `>> 3` |    8 |    `/ 8` | voxel kecil        |
// |  `>> 4` |   16 |   `/ 16` | **block → chunk**  |
// |  `>> 5` |   32 |   `/ 32` | half-region        |
// |  `>> 6` |   64 |   `/ 64` | biome grid         |
// |  `>> 7` |  128 |  `/ 128` | noise scale        |
// |  `>> 8` |  256 |  `/ 256` | map scale          |
// |  `>> 9` |  512 |  `/ 512` | **block → region** |
// | `>> 10` | 1024 | `/ 1024` | mega-region        |
// | ------- | ---- | -------- | ------------------ | 

void Location(int64_t seed, int mcVersion, char structureType[]){
    Generator g;
    int regionRadius = 3; // radius REGION (bukan chunk!)
    int structure;
    int dim;
    Pos spawn;
    int count = 0;

    if (strcmp(structureType, "village") == 0) {
        structure = Village; dim = DIM_OVERWORLD;}
    else if (strcmp(structureType, "bastion") == 0) {
        structure = Bastion; dim = DIM_NETHER;}
    else if (strcmp(structureType, "fortress") == 0) {
        structure = Fortress;  dim = DIM_NETHER;}
    else if (strcmp(structureType, "treasure") == 0) {
        structure = Treasure;  dim = DIM_OVERWORLD;}
    else if (strcmp(structureType, "ruined_portal") == 0) {
        structure = Ruined_Portal;  dim = DIM_OVERWORLD;}
    else {printf("Unknown structure type\n");return;}


    setupGenerator(&g, mcVersion, 0);
    applySeed(&g, dim, seed);

    spawn = getSpawn(&g);

    int spawnChunkX = spawn.x >> 4;
    int spawnChunkZ = spawn.z >> 4;

    // ⬇️ INI YANG PENTING
    int spawnRegionX = spawnChunkX >> 5; // /32
    int spawnRegionZ = spawnChunkZ >> 5;

    // printf("Seed: %lld\n", seed);
    // printf("Spawn: %d, %d\n", spawn.x, spawn.z);

    for (int rx = -regionRadius; rx <= regionRadius; rx++) {
        for (int rz = -regionRadius; rz <= regionRadius; rz++) {

            Pos p;
            int regionX = spawnRegionX + rx;
            int regionZ = spawnRegionZ + rz;

            if (getStructurePos(structure, mcVersion, seed, regionX, regionZ, &p)) {
                if (isViableStructurePos(structure, &g, p.x, p.z, 0)) {

                    int relX = p.x - spawn.x;
                    int relZ = p.z - spawn.z;

                    printf(
                        "%s:(%6d,%6d)\n", structureType, p.x, p.z
                    );

                    count++;
                }
            }
        }
    }

    printf("structure: %d\n", count);
}

int SpawnPlayer(int64_t seed, int Version) {
    Generator g;
    Pos spawn;

    applySeed(&g, DIM_OVERWORLD, seed);
    setupGenerator(&g, Version, 0);
    spawn = getSpawn(&g);
    printf("%d, %d\n", spawn.x, spawn.z);
}


int main(int argc, char *argv[]) {

    // Location(-7599955988224639958, MC_1_16_1, "Bastion");
    // Location(-7599955988224639958, MC_1_16_1, "Fortress");
    // Location(-7599955988224639958, MC_1_16_1, "Village");
    // Location(-7599955988224639958, MC_1_16_1, "Treasure");

    // SpawnPlayer(12391312, MC_1_16_1);  /x, z
    return 0;
}