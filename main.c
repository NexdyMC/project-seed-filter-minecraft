/*
gcc main.c cubiomes/libcubiomes.a -o main && main
*/
// inti project nexdy
// ini adalah inti code project yang dimana 
// semua sorce code yang terpisah kini bisa bergabung yang bisa menghasilkan hasil yang lebih baik

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include "cubiomes/generator.h"
#include "cubiomes/biomes.h"
#include "cubiomes/finders.h"

// typedof 
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

// location biome
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


int main(int argc, char *argv[])
{
    // === time start === //
    clock_t start = clock();
    
    // === seed === //
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
    
    fclose(fp);
    printf("File %s berhasil dibuat (%d structure) (%d biomes)\n", filename, count);
        
    clock_t end = clock();
    double waktu = (double)(end - start) / CLOCKS_PER_SEC;  
    printf("Waktu eksekusi: %f detik\n", waktu);

    return 0;
}

