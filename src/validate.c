#include "validate.h"

/* Tabel radius (dalam block) per tipe struktur.
 *
 * Nilai yang DIBERIKAN EKSPLISIT di spesifikasi:
 *   Village=12*16, Treasure=1*16, Shipwreck=2*16, Ruined_Portal=1*16,
 *   Bastion=4*16, Fortress=12*16, Mansion=6*16, Mineshaft=10*16.
 *
 * Nilai berikut TIDAK disebutkan eksplisit (masuk kategori "dsb.") dan
 * diisi dengan nilai wajar berdasarkan skala area generate structure-nya
 * di Minecraft -- SILAKAN DIUBAH di sini kalau kamu punya angka yang
 * lebih spesifik: End_City, Desert_Pyramid, Jungle_Temple, Igloo, Monument.
 */
static const int kRadiusTable[SF_STRUCT_COUNT] = {
    [SF_TREASURE]        = 1  * 16,
    [SF_VILLAGE]         = 12 * 16,
    [SF_SHIPWRECK]       = 2  * 16,
    [SF_RUINED_PORTAL]   = 1  * 16,
    [SF_MANSION]         = 6  * 16,
    [SF_BASTION]         = 4  * 16,
    [SF_FORTRESS]        = 12 * 16,
    [SF_END_CITY]        = 8  * 16,  /* default, tidak disebutkan spek */
    [SF_DESERT_PYRAMID]  = 3  * 16,  /* default, tidak disebutkan spek */
    [SF_JUNGLE_TEMPLE]   = 3  * 16,  /* default, tidak disebutkan spek */
    [SF_MINESHAFT]       = 10 * 16,
    [SF_IGLOO]           = 2  * 16,  /* default, tidak disebutkan spek */
    [SF_MONUMENT]        = 8  * 16,  /* default, tidak disebutkan spek */
};

int sf_struct_radius(SFStructureType type) {
    if ((int)type < 0 || (int)type >= SF_STRUCT_COUNT) return 0;
    return kRadiusTable[type];
}

static int iabs(int v) { return v < 0 ? -v : v; }

int sf_validate_distance(int playerX, int playerZ,
                          int structX, int structZ,
                          int radiusPlayer, int radiusStruct,
                          int *outDx, int *outDz, int *outThreshold) {
    int dx = iabs(structX - playerX);
    int dz = iabs(structZ - playerZ);
    int threshold = radiusPlayer + radiusStruct;

    if (outDx) *outDx = dx;
    if (outDz) *outDz = dz;
    if (outThreshold) *outThreshold = threshold;

    return (dx <= threshold && dz <= threshold) ? 1 : 0;
}
