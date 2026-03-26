// Febri@DESKTOP-CS39CUV MINGW64 /f/downloads
// $ gcc render_map.c cubiomes/libcubiomes.a -I cubiomes -lpng -lm -O2 -mconsole -o render_map.exe

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <png.h>

#include "cubiomes/generator.h"
#include "cubiomes/biomes.h"
#include "cubiomes/finders.h"

/* ================= CONFIG ================= */

#define MAP_SIZE   512     // ukuran gambar (px)
#define SCALE      4      // 1 pixel = 4 block
#define SPAWN_DOT  8       // ukuran titik spawn (px)

/* ================= BIOME COLOR ================= */

static inline uint32_t getBiomeColor(int biome)
{
    switch (biome)
    {
        /* ================= OCEAN ================= */
        case ocean:
        case warm_ocean:
            return 0x000070;

        case lukewarm_ocean:
            return 0x000090;

        case cold_ocean:
            return 0x202070;

        case frozen_ocean:
            return 0xA0A0FF;

        case deep_ocean:
        case deep_lukewarm_ocean:
            return 0x000030;

        case deep_cold_ocean:
            return 0x101040;

        case deep_frozen_ocean:
            return 0x5050A0;

        /* ================= RIVER ================= */
        case river:
            return 0x0000FF;

        case frozen_river:
            return 0xA0A0FF;

        /* ================= SNOW / ICE ================= */
        case snowy_plains:
            return 0xffffff;
        case snowy_taiga:
            return 0x31554a;
        case snowy_taiga_hills:
            return 0x243f36;

        case snowy_beach:
            return 0xDDEEFF;

        case ice_spikes:
            return 0xB0E0FF;
        // case snowy_mountains:
        //     return 0x31554a;
        // case wooded_mountains:
        //     return 0x7f8f7f;
        case snowy_mountains:
            return 0xa0a0a0;
            
        /* ================= DESERT ================= */
        case desert:
        case desert_hills:
            return 0xFA9418;

        /* ================= BADLANDS ================= */
        case badlands:
        case wooded_badlands:
            return 0xD94515;
        case badlands_plateau:
            return 0xca8c65;
        case modified_wooded_badlands_plateau:
            return 0xd8bf8d;
        case modified_badlands_plateau:
            return 0xf2b48d;
        case eroded_badlands:
            return 0xFF6F3F;

        /* ================= PLAINS / FOREST ================= */
        case plains:
            return 0x8DB360;
        case sunflower_plains:
            return 0xb5db88;
        // case windswept_forest:
            // return 0x22551c;
        
        case taiga:
            return 0x0b6659;
        case taiga_hills:
            return 0x163933;
        
        case forest:
            return 0x056621;
        case flower_forest:
            return 0x2d8e49;
        case dark_forest_hills:
            return 0x687942;
        
        case birch_forest:
            return 0x056621;
        case birch_forest_hills:
            return 0x13391e;

        case dark_forest:
            return 0x40511A;

        /* ================= SWAMP ================= */
        case swamp:
            return 0x07f9b2;

        /* ================= JUNGLE ================= */
        
        case jungle:
            return 0x537b09;
        case jungle_hills:
            return 0x2c4205;
        case bamboo_jungle:
            return 0x768e14;

        /* ================= SAVANNA ================= */
        case savanna:
            return 0xbdb25f;
        case savanna_plateau:
            return 0xa79d64;
        case windswept_savanna:
            return 0xe5da87;
        

        /* ================= BEACH ================= */
        case beach:
            return 0xFADE55;

        case stony_shore:
            return 0xA2A284;

        /* ================= MOUNTAINS ================= */
 
        // case windswept_hills:
            // return 0x606060;

        case mountains:
            return 0x3a3a3a;
        
        case wooded_mountains:
            return 0x507050;
        case giant_spruce_taiga_hills:
            return 0x6D7766;
        case giant_tree_taiga_hills:
            return 0x454f3e;

        case old_growth_pine_taiga:
            return 0x596651;
    

        case stony_peaks:
            return 0xAAAAAA;

        /* ================= MUSHROOM ================= */
        case mushroom_fields:
            return 0xff00ff;

        case mushroom_field_shore:
            return 0xa000ff;

        /* ================= DEFAULT ================= */
        default:
            return 0x7BA331; // fallback
    }
}


/* ================= MAIN ================= */


int main(int argc, char **argv)
{
    if (argc < 3) {
        printf("Usage: %s <seed> <version>\n", argv[0]);
        printf("Example: %s 12345678 1.16.1\n", argv[0]);
        return 1;
    }

    int64_t seed = atoll(argv[1]);

    int mc_version;
    if (!strcmp(argv[2], "1.16.1")) mc_version = MC_1_16_1;
    // else if (!strcmp(argv[2], "1.18.1")) mc_version = MC_1_18_1;
    else if (!strcmp(argv[2], "1.21")) mc_version = MC_1_21;
    else {
        printf("Unsupported version\n");
        return 1;
    }

    /* ===== Setup generator ===== */

    Generator g;
    setupGenerator(&g, mc_version, 0);
    applySeed(&g, DIM_OVERWORLD, seed);

    /* ===== Get spawn ===== */

    // int spawnX, spawnZ;
    // getSpawn(&g, &spawnX, &spawnZ);
    Pos spawn = getSpawn(&g);
    int spawnX = spawn.x;
    int spawnZ = spawn.z;

    printf("Spawn at: X=%d Z=%d\n", spawnX, spawnZ);

    /* ===== Create PNG ===== */

    FILE *fp = fopen("map.png", "wb");
    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info = png_create_info_struct(png);
    png_init_io(png, fp);

    png_set_IHDR(
        png, info,
        MAP_SIZE, MAP_SIZE,
        8,
        PNG_COLOR_TYPE_RGB,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT
    );

    png_write_info(png, info);

    png_bytep row = malloc(3 * MAP_SIZE);

    /* ===== Render Map ===== */

    for (int z = 0; z < MAP_SIZE; z++) {
        for (int x = 0; x < MAP_SIZE; x++) {

            int blockX = (x - MAP_SIZE / 2) * SCALE;
            int blockZ = (z - MAP_SIZE / 2) * SCALE;

            int biome = getBiomeAt(&g, SCALE, blockX, 64, blockZ);
            uint32_t c = getBiomeColor(biome);

            /* ===== Spawn marker ===== */
            int px = (spawnX / SCALE) + MAP_SIZE / 2;
            int pz = (spawnZ / SCALE) + MAP_SIZE / 2;

            if (abs(x - px) < SPAWN_DOT / 2 &&
                abs(z - pz) < SPAWN_DOT / 2)
            {
                c = 0xFF0000; // RED spawn dot
            }

            row[x*3+0] = (c >> 16) & 255;
            row[x*3+1] = (c >> 8) & 255;
            row[x*3+2] = c & 255;
        }
        png_write_row(png, row);
    }

    png_write_end(png, NULL);
    fclose(fp);
    free(row);

    printf("map.png generated\n");
    return 0;
}
