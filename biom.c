/* 
gcc biom.c cubiomes/libcubiomes.a -o biom && biom 8899962566774883052 ocean
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "cubiomes/generator.h"
#include "cubiomes/biomes.h"

int main(int argc, char *argv[])
{
    long long seed = -2978558753374473912;
    printf("Seed: %lld\n", seed);
    
    
    Generator g;
    setupGenerator(&g, MC_1_16_1, 0);
    applySeed(&g, DIM_OVERWORLD, seed);

    int totalBiome = 0;
    
    
    int radius = 512; // block 
    printf("Mencari biome dalam radius %d block...\n", radius);

    for (int X = -radius; X <= radius; X += 16)
    {
        for (int Z = -radius; Z <= radius; Z += 16)
        {
            int biomeID = getBiomeAt(&g, 1, X, 0, Z);
            
            if (biomeID == ocean)
            {
                printf("ocean ditemukan di X=%d Z=%d\n", X, Z);

            }
        }
    }

    printf("data sudah ditemukan");
}