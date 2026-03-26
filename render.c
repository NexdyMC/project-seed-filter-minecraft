#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int main(int argc, char *argv[])
{
    printf("Hello World %s!!", argv[1]);
    // code area 
    // 
    int search = 0;
    int radius = 96;
    
    // Treasure 100%
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
                }
            }
        }
    }

    // Village 100%
    int radius = 4; // 1 region : 512 chunk
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
                }
            }
        }
    }

    // Shipwreck 50%
    int radius = 4; // 4 region : 512 chunk
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
                }
                count++;
            }
        }
    }
    
    // ruind portal 50%
    int radius = 1; // 1 region : 512 chunk
    int centerX = 0 >> 4;
    int centerZ = 0 >> 4;


    for (int rx = centerX - radius; rx <= centerX + radius; rx++)
    {
        for (int rz = centerZ - radius; rz <= centerZ + radius; rz++)
        {

            Pos p;

            if (getStructurePos(Ruined_Portal, MC_1_16_5, seed, rx, rz, &p))
            {
                if (isViableStructurePos(Ruined_Portal, &g, p.x, p.z, 0))
                {
                    printf("Ruined_Portal at (%d, %d)\n", p.x + 8, p.z + 8 );
                }
                count++;
            }
        }
    }

    // bastion 
    int radius = 3; // 1 region : 512
    // masalahnya radius 3 menjadi radius 2.5
    int centerX = 0 >> 4;
    int centerZ = 0 >> 4;


    for (int rx = centerX - radius; rx <= centerX + radius; rx++)
    {
        for (int rz = centerZ - radius; rz <= centerZ + radius; rz++)
        {

            Pos p;

            if (getStructurePos(Bastion, MC_1_16_5, seed, rx, rz, &p))
            {
                if (isViableStructurePos(Bastion, &g, p.x, p.z, 0))
                {
                    printf("Bastion at (%d, %d)\n", p.x + 8, p.z + 8 );
                }
                count++;
            }
        }
    }

    
    // fortress 60%
    int radius = 1 + 1; // 1 region : 512 chunk
    
    int centerX = 0 ;
    int centerZ = 0 ;


    for (int rx = centerX - radius; rx < centerX + radius; rx++)
    {
        for (int rz = centerZ - radius; rz < centerZ + radius; rz++)
        {

            Pos p;

            if (getStructurePos(Fortress, MC_1_16_5, seed, rx, rz, &p))
            {
                if (isViableStructurePos(Fortress, &g, p.x, p.z, 0))
                {
                    printf("Fortress at (%d, %d)\n", p.x, p.z  );
                    count++;
                }
            }
        }
    }

    // ruind portal nether 60%
    int radius = 2 ; // 1 region : 512 chunk
    
    int centerX = 0 ;
    int centerZ = 0 ;


    for (int rx = centerX - radius; rx < centerX + radius; rx++)
    {
        for (int rz = centerZ - radius; rz < centerZ + radius; rz++)
        {

            Pos p;

            if (getStructurePos(Ruined_Portal_N, MC_1_16_5, seed, rx, rz, &p))
            {
                if (isViableStructurePos(Ruined_Portal_N, &g, p.x, p.z, 0))
                {
                    printf("Ruined_Portal_N at (%d, %d)\n", p.x, p.z  );
                    count++;
                }
            }
        }
    }

}