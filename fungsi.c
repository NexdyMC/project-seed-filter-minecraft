/* 
gcc fungsi.c cubiomes/libcubiomes.a -o fungsi && fungsi 

membuat fungsi untuk mencari structure
selesai
*/

// membuat fungsi untuk mencari biomes

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>


#include "cubiomes/generator.h"
#include "cubiomes/biomes.h"
#include "cubiomes/finders.h"

typedef struct {
    char type[20];
    int x;
    int z;
} Data;


// location structure
const char* LocationStructure(FILE *fp, long long seed, int mc, int StructureType, char *type, int radius, int dimension ) 
{
    Generator g;
    Pos spawn = getSpawn(&g);
    setupGenerator(&g, mc, 0);
    applySeed(&g, dimension, seed);
    for (int rx = -radius; rx <= radius; rx++)
    {
        for (int rz = -radius; rz <= radius; rz++)
        {
            Pos p;

            if (getStructurePos(StructureType, mc, seed, rx, rz, &p))
            {
                if (isViableStructurePos(StructureType, &g, p.x, p.z, 0))
                {
                    printf("%s at (%d, %d)\n", type, p.x, p.z);
                    fprintf(fp, "\t\t{\"type\": \"%s\", \"x\": %d, \"z\": %d},\n", type, p.x, p.z);
                }
            }
        }
    }
}

const char* LocationBiome(FILE *fp, long long seed, int mc, int BiomeType, char *type, int radius, int dimension ) 
{   
    Generator g;
    setupGenerator(&g, mc, 0);
    applySeed(&g, dimension, seed);
    printf("Mencari biome %s dalam radius %d block...\n", type,radius);

    Data res;

    for (int X = -radius; X <= radius; X += 16)
    {
        for (int Z = -radius; Z <= radius; Z += 16)
        {
            int biomeID = getBiomeAt(&g, 1, X, 0, Z);
            
            if (biomeID == BiomeType)
            {
                // printf("%s location X= %d Z= %d\n", type, X, Z);
                // fprintf(fp, "\t\t{\"type\": \"%s\", \"x\": %d, \"z\": %d},\n", type, X, Z);
                strcpy(res.type, type); // ✅ BENAR
                res.x = X;
                res.z = Z;

                fwrite(&res, sizeof(Data), 1, fp); // ✅ BENAR
            }
        }
    }
}

int main(int argc, char *argv[]) {
    clock_t start = clock();

    long long seed = atoll(argv[1]);
    printf("Seed: %lld\n", seed);
    int mc = MC_1_16_1;
    Generator g;

    // === create file json === //
    char FileJsonName[100];
    sprintf(FileJsonName, "seed/%lld.json", seed);
    FILE *cjson = fopen(FileJsonName, "w");
    
    // === create file bat === //
    char FileBinName[100];
    sprintf(FileBinName, "seed/%lld.bin", seed);
    FILE *cbin = fopen(FileBinName, "wb");


    // ===== Write JSON ===== //
    fprintf(cjson, "{\n");
    fprintf(cjson, "  \"seed\": %lld,\n", seed);
    fprintf(cjson, "  \"version\": \"1.16.1\",\n");
    fprintf(cjson, "  \"spawn\": [%d, %d],\n", getSpawn(&g).x, getSpawn(&g).z);
    
    fprintf(cjson, "  \"structures\": [\n");
    LocationStructure(cjson, seed, mc, Treasure, "Treasure", 16, DIM_OVERWORLD);
    LocationStructure(cjson, seed, mc, Shipwreck, "Shipwreck", 2, DIM_OVERWORLD);
    LocationStructure(cjson, seed, mc, Village, "Village", 2, DIM_OVERWORLD);
    LocationStructure(cjson, seed, mc, Monument, "Monument", 2, DIM_OVERWORLD);
    LocationStructure(cjson, seed, mc, Bastion, "Bastion", 1, DIM_NETHER);
    LocationStructure(cjson, seed, mc, Fortress, "Fortress", 1, DIM_NETHER);
    fprintf(cjson, "\t\t{\"type\": \"break\", \"x\": 0, \"z\": 0}\n");
    fprintf(cjson, "  ],\n");
    
    LocationBiome(cbin, seed, mc, ocean, "Ocean", 512, DIM_OVERWORLD);
    LocationBiome(cbin, seed, mc, plains, "Plains", 512, DIM_OVERWORLD);
    fprintf(cjson, "  \"biomes\": \"seed/%lld.bin\"\n", seed);


    // === penutup code === //
    fprintf(cjson, "}\n");
    fclose(cbin);
    fclose(cjson);
    
    clock_t end = clock();
    double waktu = (double)(end - start) / CLOCKS_PER_SEC;  
    printf("Waktu eksekusi: %f detik\n", waktu);


    return 0;
}