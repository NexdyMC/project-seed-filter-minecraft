/*
    gcc struc.c cubiomes/libcubiomes.a -o struc && struc 8899962566774883052
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
    printf("Seed: %lld\n", seed);
    
    char filename[100];
    sprintf(filename, "seed/%lld.json", seed);
    FILE *fp = fopen(filename, "w");
    

    if (!fp) {
        printf("folder \"seed\" Failed to open file!\n");
        return 1;
    }

    int count = 0;    
    Generator g;
    Pos spawn = getSpawn(&g);
    setupGenerator(&g, MC_1_16_1, 0);
    applySeed(&g, DIM_OVERWORLD, seed);

    fprintf(fp, "{\n");
    fprintf(fp, "  \"seed\": %lld,\n", seed);
    fprintf(fp, "  \"version\": \"1.16.1\",\n");
    fprintf(fp, "  \"spawn\": [%d, %d],\n", spawn.x, spawn.z);
    fprintf(fp, "  \"structures\": [\n");
    
    int search = 0;
    int centerX = 0 ;
    int centerZ = 0 ;
    
    int radius = 4 * 12; // 1 region : 512 chunk
    // treasure
    for (int rx = -radius; rx <= radius; rx++)
    {
        for (int rz = -radius; rz <= radius; rz++)
        {
            Pos p;

            if (getStructurePos(Treasure, MC_1_16_1, seed, rx, rz, &p))
            {
                if (isViableStructurePos(Treasure, &g, p.x, p.z, 0))
                {
                    printf("Treasure at (%d, %d)\n", p.x, p.z);
                    fprintf(fp,"    { \"type\": \"%s\", \"x\": %d, \"z\": %d },\n", "Treasure", p.x, p.z );
                }
            }
        }
    }
    
    radius = 2;
    for (int rx = -radius; rx < radius; rx++)
    {
        for (int rz = -radius; rz < radius; rz++)
        {
            Pos p;
            
            if (getStructurePos(Shipwreck, MC_1_16_1, seed, rx, rz, &p))
            {
                if (isViableStructurePos(Shipwreck, &g, p.x, p.z, 0))
                {
                    printf("Shipwreck at (%d, %d)\n", p.x + 8, p.z + 8);
                    fprintf(fp,"    { \"type\": \"%s\", \"x\": %d, \"z\": %d },\n", "Shipwreck", p.x, p.z );
                }
                count++;
            }
        }
    }
    

    // village
    for (int rx = -radius; rx < radius; rx++)
    {
        for (int rz = -radius; rz < radius; rz++)
        {
            Pos p;

            if (getStructurePos(Village, MC_1_16_1, seed, rx, rz, &p))
            {
                if (isViableStructurePos(Village, &g, p.x, p.z, 0))
                {
                    printf("Village at (%d, %d)\n", p.x + 8, p.z + 8);
                    fprintf(fp,"    { \"type\": \"%s\", \"x\": %d, \"z\": %d },\n", "Village", p.x, p.z );
                }
            }
        }
    }

    // bastion
    // for (int rx = centerX - radius; rx <= centerX + radius; rx++)
    // {
    //     for (int rz = centerZ - radius; rz <= centerZ + radius; rz++)
    //     {

    //         Pos p;

    //         if (getStructurePos(Bastion, MC_1_16_5, seed, rx, rz, &p))
    //         {
    //             if (isViableStructurePos(Bastion, &g, p.x, p.z, 0))
    //             {
    //                 printf("Bastion at (%d, %d)\n", p.x + 8, p.z + 8 );
    //             }
    //             count++;
    //         }
    //     }
    // }

    // // fortress
    // for (int rx = centerX - radius; rx < centerX + radius; rx++)
    // {
    //     for (int rz = centerZ - radius; rz < centerZ + radius; rz++)
    //     {

    //         Pos p;

    //         if (getStructurePos(Fortress, MC_1_16_5, seed, rx, rz, &p))
    //         {
    //             if (isViableStructurePos(Fortress, &g, p.x, p.z, 0))
    //             {
    //                 printf("Fortress at (%d, %d)\n", p.x, p.z  );
    //                 count++;
    //             }
    //         }
    //     }
    // }
    fprintf(fp,"    { \"type\": \"break\", \"x\": 0, \"z\": 0 }\n", "");
    fprintf(fp, "  ]\n");
    fprintf(fp, "}\n");
    fclose(fp);
    printf("File %s berhasil dibuat (%d structure) (%d biomes)\n",filename);

    printf("structure: %d %d\n", count, search);

    return 0;
}

// z = -68 47