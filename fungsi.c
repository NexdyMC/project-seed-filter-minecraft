/* 
gcc fungsi.c cubiomes/libcubiomes.a -o fungsi && fungsi 1234567890

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
} list_structure;

typedef struct {
    char type[20];
    int x;
    int z;
} list_biome;

/// === metode File === ///
// location structure
const char* LocationStructureJson(FILE *fp, long long seed, int mc, int StructureType, char *type, int radius, int dimension ) 
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

const char* LocationStructureBin(FILE *file, long long seed, int mc, int StructureType, char *type, int radius, int dimension ) 
{
    Generator g;
    Pos spawn = getSpawn(&g);
    setupGenerator(&g, mc, 0);
    applySeed(&g, dimension, seed);

    list_structure res;

    for (int rx = -radius; rx <= radius; rx++)
    {
        for (int rz = -radius; rz <= radius; rz++)
        {
            Pos p;

            if (getStructurePos(StructureType, mc, seed, rx, rz, &p))
            {
                if (isViableStructurePos(StructureType, &g, p.x, p.z, 0))
                {
                    // printf("%s at (%d, %d)\n", type, p.x, p.z);
                    // fprintf(fp, "\t\t{\"type\": \"%s\", \"x\": %d, \"z\": %d},\n", type, p.x, p.z);
                    strcpy(res.type, type); 
                    res.x =  p.x;
                    res.z =  p.z;

                    fwrite(&res, sizeof(list_structure), 1, file);
                }
            }
        }
    }
}

const char* LocationBiomeBin(FILE *file, long long seed, int mc, int BiomeType, char *type, int radius, int dimension ) 
{   
    Generator g;
    setupGenerator(&g, mc, 0);
    applySeed(&g, dimension, seed);
    printf("Mencari biome %s dalam radius %d block...\n", type, radius);

    list_biome res;

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

                fwrite(&res, sizeof(list_biome), 1, file); // ✅ BENAR
            }
        }
    }
}

typedef struct {
    char nama[20];
    int x;
    int z;
} Structure;

#define MAX_DATA 1000

int CountStructure = 0;k 

Structure Bmaps[MAX_DATA];

/// === metode array map === ///
const char LocationStructureMap(Structure, long long seed, int mc, int StructureType, char *type, int radius, int dimension ) 
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
    
                    strcpy(Bmap[CountStructure].nama, type);
                    Bmaps[CountStructure].x = p.x;
                    Bmaps[CountStructure].z = p.z;
                    CountStructure++;

                }
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
    sprintf(FileJsonName, "seed/structures/s%lld.bin", seed);
    FILE *structureBin = fopen(FileJsonName, "w");
    
    // === create file bat === //
    char FileBinName[100];
    sprintf(FileBinName, "seed/biomes/b%lld.bin", seed);
    FILE *biomeBin = fopen(FileBinName, "wb");

    
    
    // ===== Write JSON ===== //

    // LocationStructureBin(structureBin, seed, mc, Village, "Village", 2, DIM_OVERWORLD);
    // LocationStructureBin(structureBin, seed, mc, Treasure, "Treasure", 16, DIM_OVERWORLD);
    // LocationStructureBin(structureBin, seed, mc, Shipwreck, "Shipwreck", 2, DIM_OVERWORLD);
    // LocationStructureBin(structureBin, seed, mc, Bastion, "Bastion", 2, DIM_NETHER);
    // LocationStructureBin(structureBin, seed, mc, Fortress, "Fortress", 2, DIM_NETHER);

    LocationStructureMap(seed, mc, Village, "Village", 2, DIM_OVERWORLD);

    // LocationBiomeBin(biomeBin, seed, mc, ocean, "Ocean", 512, DIM_OVERWORLD);
    // LocationBiomeBin(biomeBin, seed, mc, plains, "Plains", 512, DIM_OVERWORLD);


    // === penutup code === //
    fclose(structureBin);

    fclose(biomeBin);

    // === validasi jarak === ///
    
    // === tampilkan output === //
    for (int i = 0; i < CountStructure; i++) {
        printf("%d. Nama: %s\n", i, Bmaps[i].nama);
    }

    
    clock_t end = clock();
    double waktu = (double)(end - start) / CLOCKS_PER_SEC;  
    printf("Waktu eksekusi: %f detik\n", waktu);


    return 0;
}