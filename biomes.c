/* 
gcc biomes.c cubiomes/libcubiomes.a -o biomes && biomes 8899962566774883052 ocean
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "cubiomes/generator.h"
#include "cubiomes/biomes.h"


typedef struct {
    const char *name;
    int type;
} BiomeMap;

typedef struct {
    char type[32];
    int x;
    int z;
} Biomes;

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Usage: program <seed> --help\n");
        return 1; 
    }

    int radius = 512; // block 
    for (int i = 1; i < argc; i++) 
    { 

        // parameter : informasi
        if (strcmp(argv[i], "--help") == 0) {
            printf("Usage: %s <seed: int> [List Biomes] [options]\n\n", argv[0]);

            printf("Default: \n\tminecraft version: 1.16.1\n\n");

            printf("Options:\n");
            printf("\t--help                    tampilkan sebuah informasi.\n");
            printf("\t--radius:  | int |        mengatur jarak pencaharian dalam chunk radius.\n");
            printf("\n");
            
            printf("List Biomes:\n");
            printf("\t ocean        \n");
            printf("\t deep_ocean   \n");
            printf("\t beach        \n");
            printf("\t plains       \n");
            printf("\t desert       \n");
            return 1;
            break;
        }

        if (strncmp(argv[i], "--radius:", 9) == 0) {
            char *angka_str = argv[i] + 9;
            radius = atoi(angka_str);
            printf("jarak berada di %d\n", radius);
        }

        if (strncmp(argv[i], "-v:", 3) == 0 ) {
            char *angka_str = argv[i] + 3;
            radius = atoi(angka_str);
            printf("jarak berada di %d\n", radius);
        }
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
    
    

    BiomeMap BMap[] = {
        {"ocean", ocean},
        {"beach", beach},
        {"plains", plains},
        {"desert", desert},
        {"deep_ocean", deep_ocean},
    };
    


    int mapCount = sizeof(BMap) / sizeof(BMap[0]);

    Biomes biomes[5000];
    
    int totalBiome = 0;

    for (int i = 2; i < argc; i++)   
    {
        
        for (int m = 0; m < mapCount; m++)
        {
            if (strcmp(argv[i], BMap[m].name) == 0)
            {
                printf("Mencari biome %s dalam radius %d block...\n", argv[i], radius);

                for (int X = -radius; X <= radius; X += 16)
                {
                    for (int Z = -radius; Z <= radius; Z += 16)
                    {
                        int biomeID = getBiomeAt(&g, 1, X, 0, Z);
                        
                        if (biomeID == BMap[m].type)
                        {
                            if (totalBiome < 5000) {
                                printf("%s ditemukan di X=%d Z=%d\n", BMap[m].name, X, Z);
                                strcpy(biomes[totalBiome].type, BMap[m].name);
                                biomes[totalBiome].x = X;
                                biomes[totalBiome].z = Z;
                                totalBiome++;
                            }
                        }
                    }
                }
            }
        }
    }

    fprintf(fp, "{\n");
    fprintf(fp, "  \"seed\": %lld,\n", seed);
    fprintf(fp, "  \"version\": \"1.16.1\",\n");
    
    // === Biomes JSON ===
    fprintf(fp, "  \"biomes\": [\n");
    for (int i = 0; i < totalBiome; i++) {
        fprintf(fp,
            "    { \"type\": \"%s\", \"x\": %d, \"z\": %d }",
            biomes[i].type, biomes[i].x, biomes[i].z
        );

        if (i < totalBiome - 1)
            fprintf(fp, ",");

        fprintf(fp, "\n");
    }
    fprintf(fp, "  ]\n");

    fprintf(fp, "}\n");
    fclose(fp);

    printf("File %s berhasil dibuat (%d biomes)\n",
           filename, totalBiome);

    return 0;
}
