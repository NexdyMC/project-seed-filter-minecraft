/*
gcc end_city.c cubiomes/libcubiomes.a -o end_city && end_city 123123
gcc end_city.c cubiomes/libcubiomes.a -o city && city 123123
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "cubiomes/generator.h"
#include "cubiomes/biomes.h"
#include "cubiomes/finders.h"


int main(int argc, char *argv[])
{
    int64_t seed = atoll(argv[1]);  
    // printf("Seed: %lld version 1.16.1\n", seed);
    printf("Seed: %lld version 1.21.3\n", seed);

    int count = 0;
    
    Generator g;

    Pos spawn = getSpawn(&g);
    setupGenerator(&g, MC_1_21_3, 0);
    applySeed(&g, DIM_END, seed);
 
    int search = 0;
    
    // 60%
    
    
    int centerX = 0 ;
    int centerZ = 0 ;
    
    printf("mencari end city dalam radius 1\n");
    int radius = 8; // 1 region : 512 chunk
    // treasure
    for (int rx = -radius; rx <= radius; rx++)
    {
        for (int rz = -radius; rz <= radius; rz++)
        {
            Pos p;

            if (getStructurePos(End_City, MC_1_16_1, seed, rx, rz, &p))
            {
                if (isViableStructurePos(End_City, &g, p.x, p.z, 0))
                {
                    printf("End_City at /tp %d ~ %d\n", p.x, p.z);
                }
            }
        }
    }

    printf("pencarian telah berhenti");
    
}