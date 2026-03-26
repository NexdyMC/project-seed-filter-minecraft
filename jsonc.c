// gcc jsonc.c cubiomes/libcubiomes.a -o jsonc
//  
// analisis jarak structure mendeterksi dengan radius 1 region dan chunk

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include "cubiomes/generator.h"
#include "cubiomes/biomes.h"
#include "cubiomes/finders.h"

#define MAX_DATA 100

typedef struct {
    char type[32];
} s_list;

typedef struct {
    const char *name;
    int type;
    int dimensi;
} MapStructure;

typedef struct {
    char type[32];
    int x;
    int z;
} Structure;

s_list S_list[MAX_DATA];
int s_count = 0;


int main(int argc, char *argv[]) 
{
    clock_t start = clock();

    if (argc < 2) {
        printf("Usage: program <seed> <structure> <biomes>\n");
        return 1;
    }

    long long seed = atoll(argv[1]);
    printf("Seed: %lld\n", seed);

    char filename[100];
    sprintf(filename, "seed/%lld.json", seed);
    FILE *fp = fopen(filename, "w");
    
    if (!fp) {
        printf("folder \"seed\" Failed to open file!\n");
        return 1;
    }
    
    Generator g;
    setupGenerator(&g, MC_1_16_1, 0);
    applySeed(&g, DIM_OVERWORLD, seed);

    Pos spawn = getSpawn(&g);
    int spawnRegionX = spawn.x >> 9;
    int spawnRegionZ = spawn.z >> 9;


    MapStructure Smaps[] = {
        {"village", Village, DIM_OVERWORLD},
        {"ruind_portal", Ruined_Portal, DIM_OVERWORLD},
        {"shipwreck", Shipwreck, DIM_OVERWORLD},
        {"mansion", Mansion, DIM_OVERWORLD},
        {"bastion", Bastion, DIM_NETHER},
        {"fortress", Fortress, DIM_NETHER},
    };
    
    
    int mapCount = sizeof(Smaps) / sizeof(Smaps[0]);
    
    Structure structures[512];
    
    int Structotal = 0;
    int Biometotal = 0;
    
    int radius = 512;
    for (int i = 1; i < argc; i++) 
    { 
        if (strncmp(argv[i], "--radius:", 9) == 0) {
            char *angka_str = argv[i] + 9;
            radius = atoi(angka_str);
            printf("jarak berada di %d\n", radius);
        }
    }

    // ===== Collect Structures =====
    printf("Mencari Structure dalam radius %d block...\n", radius);

    for (int i = 2; i < argc; i++)   {
        
        if (strcmp(argv[i], "treasure") == 0) {
            
            for (int cx = (spawn.x >> 4) - 20; cx <= (spawn.x >> 4) + 20; cx++)
            {
                for (int cz = (spawn.z >> 4) - 20; cz <= (spawn.z >> 4) + 20; cz++)
                {
                    Pos p;
                    if (getStructurePos(Treasure, MC_1_16_1, seed, cx, cz, &p))
                    {
                        if (isViableStructurePos(Treasure, &g, p.x, p.z, 0))
                        {
                            if (Structotal < 512)
                            {
                                strcpy(structures[Structotal].type, "treasure");
                                structures[Structotal].x = p.x;
                                structures[Structotal].z = p.z;
                                Structotal++;
                            }
                        }
                    }
                }
            }
        }
        
        for (int m = 0; m < mapCount; m++)
        {
            // structure tambahan

            if (strcmp(argv[i], Smaps[m].name) == 0)
            {
                applySeed(&g, Smaps[m].dimensi, seed);

                int regionRadius = radius / 512;

                for (int rx = -regionRadius; rx <= regionRadius; rx++)
                {
                    for (int rz = -regionRadius; rz <= regionRadius; rz++)
                    {
                        Pos p;
                        int regionX = spawnRegionX + rx;
                        int regionZ = spawnRegionZ + rz;

                        if (getStructurePos(Smaps[m].type, MC_1_16_1, seed, regionX, regionZ, &p)) 
                        {
                            if (isViableStructurePos(Smaps[m].type, &g, p.x, p.z, 0))
                            {
                                if (Structotal < 512)
                                {
                                    strcpy(structures[Structotal].type, Smaps[m].name);
                                    structures[Structotal].x = p.x;
                                    structures[Structotal].z = p.z;
                                    Structotal++;
                                }
                            }
                        }
                    }
                }
                strcpy(S_list[s_count].type, Smaps[m].name);
                s_count++;
            } 
        }
    }


    // ===== Write JSON =====
    fprintf(fp, "{\n");
    fprintf(fp, "  \"seed\": %lld,\n", seed);
    fprintf(fp, "  \"version\": \"1.16.1\",\n");
    fprintf(fp, "  \"spawn\": [%d, %d],\n", spawn.x, spawn.z);

    for (int i = 0; i < Structotal; i++)
    {
        for (int m = 1; m < 5; m++)
        {            
            /*
                if (
                    strcmp(structures[i].type, "treasure")      == 0 ||
                    strcmp(structures[i].type, "shipwreck")     == 0 ||
                    strcmp(structures[i].type, "village")       == 0 ||
                    strcmp(structures[i].type, "ruind_portal")  == 0 ||
                    strcmp(structures[i].type, "monument")      == 0 ||
                    strcmp(structures[i].type, "bastion")       == 0 ||
                    strcmp(structures[i].type, "fortress")      == 0
                ) 
            */
            if 
            (
                strcmp(structures[i].type, Smaps[m].name ) == 0
            )
            {
                int exists = 0;
    
                // cek apakah sudah ada di S_list
                for (int k = 0; k < s_count; k++) {
                    if (strcmp(S_list[k].type, structures[i].type) == 0) {
                        exists = 1;
                        break;
                    }
                }
    
                // kalau belum ada → baru masukin
                if (!exists) {
                    strcpy(S_list[s_count].type, structures[i].type);
                    s_count++;
                }
            }
        }
    }

    // === structure JSON ===
    fprintf(fp, "  \"structures\": [\n");
    for (int i = 0; i < Structotal; i++) {
        fprintf(fp,
            "    { \"type\": \"%s\", \"x\": %d, \"z\": %d }",
            structures[i].type, structures[i].x, structures[i].z
        );

        if (i < Structotal - 1)
            fprintf(fp, ",");

        fprintf(fp, "\n");
    }
    fprintf(fp, "  ]\n");
    
    fprintf(fp, "}\n");
    fclose(fp);

    printf("File %s berhasil dibuat (%d structure) (%d biomes)\n",
           filename, Structotal);
        
    for (int i = 0; i < s_count; i++) {
        printf("%s ", S_list[i].type);
    }

    clock_t end = clock();
    double waktu = (double)(end - start) / CLOCKS_PER_SEC;  
    printf("Waktu eksekusi: %f detik\n", waktu);

    return 0;
}

// -5585920236775604136
// 1488807202407832586