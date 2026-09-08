#ifndef SF_OUTPUT_H
#define SF_OUTPUT_H

#include "seedfilter.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Nama string untuk tiap tipe struktur/bioma (dipakai di JSON & log). */
const char *sf_structure_type_name(SFStructureType t);
const char *sf_biome_type_name(SFBiomeType b);

/* Implementasi aktual dari seed_result_to_json / search_output_to_json
 * (dideklarasikan publik di seedfilter.h) ada di output.c. Tidak perlu
 * dideklarasikan ulang di sini karena sudah SF_API di seedfilter.h. */

#ifdef __cplusplus
}
#endif

#endif /* SF_OUTPUT_H */
