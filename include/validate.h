#ifndef SF_VALIDATE_H
#define SF_VALIDATE_H

#include "seedfilter.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Radius ambang batas (dalam block) untuk tiap tipe struktur, sesuai
 * tabel di spesifikasi. Untuk struktur yang tidak disebutkan eksplisit
 * (End_City, Desert_Pyramid, Jungle_Temple, Igloo, Monument) dipakai
 * nilai default yang wajar -- TANDAI/ubah di validate.c kalau perlu
 * nilai lain. */
int sf_struct_radius(SFStructureType type);

/* Hitung jarak Manhattan antara posisi struktur dan posisi player, lalu
 * bandingkan terhadap ambang batas = radiusPlayer + radiusStruct.
 * Struktur lolos jika |dx| <= threshold DAN |dz| <= threshold.
 * Mengembalikan 1 jika lolos, 0 jika tidak. Nilai dx, dz, threshold
 * dikeluarkan lewat out-param (boleh NULL kalau tidak dibutuhkan). */
int sf_validate_distance(int playerX, int playerZ,
                          int structX, int structZ,
                          int radiusPlayer, int radiusStruct,
                          int *outDx, int *outDz, int *outThreshold);

#ifdef __cplusplus
}
#endif

#endif /* SF_VALIDATE_H */
