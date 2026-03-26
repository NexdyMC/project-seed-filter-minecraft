#include <stdio.h>
/*
SpawnRadiusStructure(
    int Spawn_x,
    int Spawn_z,
    char radius_type,
    int radius_int,
    int Structure_x,
    int Structure_z
)
Parameters:
    Spawn_x, Spawn_z : koordinat titik spawn
    radius_type      : tipe radius ('B' = block, 'C' = chunk)
    radius_int      : nilai radius (integer)
    Structure_x, Structure_z : koordinat struktur yang akan dicek

Output/Return:
    1 : struktur berada di dalam radius
    0 : struktur berada di luar radius

radius_type:
    'B' = Block radius
    'C' = Chunk radius (1 chunk = 16 block)
==============================================
normalizeRadiusType(
    const char *r
)
    
Parameters:
    r : string input radius type

Output/Return:
    'B' : Block radius
    'C' : Chunk radius

================================================
    */

char normalizeRadiusType(
    const char *r
) {
    if (!r || r[0] == '\0') return 0;

    char c = r[0];
    if (c >= 'a' && c <= 'z') c -= 32; // uppercase

    if (c == 'B') return 'B';
    if (c == 'C') return 'C';

    return 0;
}

int SpawnRadiusStructure(
    int Spawn_x,
    int Spawn_z,
    char radius_type,
    int radius_int,
    int Structure_x,
    int Structure_z
) {
    int block;
    
    if (radius_type == 'B' || radius_type == 'b') {
        block = radius_int;
    }
    else if (radius_type == 'C' || radius_type == 'c') {
        block = radius_int << 4;
    }
    else {return 0 ;}


    // atur posisi structure dari titik spawn nol (0, 0)
    // konversi dari negatif ke positif
    int location_X = (Structure_x - Spawn_x) < 0 ? -(Structure_x - Spawn_x) : (Structure_x - Spawn_x);
    int location_Z = (Structure_z - Spawn_z) < 0 ? -(Structure_z - Spawn_z) : (Structure_z - Spawn_z);

    // cek apakah structure berada di dalam radius
    if (location_X >= block) return 0;
    if (location_Z >= block) return 0;
    return 1;

}

