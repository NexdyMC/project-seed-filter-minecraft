// gcc perhitungan_json.c cJSON.c -o main && main
// max jarak radius perstructure 
// bagian perhitungan player dengan structures dan pemanpiln output
// versi json
// selesai 


#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"
#include <time.h>
#include <stdint.h>
#include <string.h>

#define MAX_DATA 10

typedef struct {
    char nama[20];
} vaidasi_data;

vaidasi_data Bmaps[MAX_DATA];

char* readFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) return NULL;

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char* data = (char*)malloc(size + 1);
    fread(data, 1, size, file);
    data[size] = '\0';

    fclose(file);
    return data;
}

int TypeStructureRadiusChunk(char *type) {
    int object_r = 16;
    if (strcmp("Village",           type) == 0) object_r = 12   * 16;
    if (strcmp("Treasure",          type) == 0) object_r = 1    * 16;
    if (strcmp("Shipwreck",         type) == 0) object_r = 2    * 16;
    if (strcmp("Ruind_portal",      type) == 0) object_r = 1    * 16;
    if (strcmp("Desert",            type) == 0) object_r = 2    * 16;
    if (strcmp("Jungle",            type) == 0) object_r = 2    * 16;
    if (strcmp("Minesharft",        type) == 0) object_r = 10   * 16;
    if (strcmp("Bastion",           type) == 0) object_r = 4    * 16;
    if (strcmp("Fortress",          type) == 0) object_r = 12   * 16;
    if (strcmp("Igloo",             type) == 0) object_r = 1    * 16;
    if (strcmp("Ruined_Portal_N",   type) == 0) object_r = 1    * 16;
    if (strcmp("Mansion",           type) == 0) object_r = 6    * 16;
    return object_r;
}

int main() {
    int count = 0;
    clock_t start = clock();

    char * jsonData = readFile("data.json");

    if (!jsonData) {
        printf("Gagal membaca file\n");
        return 1;
    }

    cJSON * json = cJSON_Parse(jsonData);

    if (!json) {
        printf("JSON error\n");
        return 1;
    }

    // Ambil data

    cJSON *structure = cJSON_GetObjectItem(json, "structures");
    cJSON *spawn_x = cJSON_GetArrayItem(cJSON_GetObjectItem(json, "player"), 0);
    cJSON *spawn_z = cJSON_GetArrayItem(cJSON_GetObjectItem(json, "player"), 1);
    
    printf("player: %d, %d \n", spawn_x->valueint, spawn_z->valueint);

    int player_x = spawn_x->valueint;
    int player_z = spawn_z->valueint;

    int s_size = cJSON_GetArraySize(structure);
    printf("structure: %d\n", s_size);

    for (int i = 0; i < s_size; i++) {

        // variaber type data
        char *type =    cJSON_GetObjectItem(cJSON_GetArrayItem(cJSON_GetObjectItem(json, "structures"), i), "type")->valuestring;
        int x_structure=cJSON_GetObjectItem(cJSON_GetArrayItem(cJSON_GetObjectItem(json, "structures"), i), "x")->valueint;
        int z_structure=cJSON_GetObjectItem(cJSON_GetArrayItem(cJSON_GetObjectItem(json, "structures"), i), "z")->valueint;        

        // cetak info
        if (strcmp("break", type) == 0) break;

        int object_r = TypeStructureRadiusChunk(type);
        
        // proses pengecekan data
        printf("- %d %s %d %d r: %d\n", i, type, x_structure, z_structure, object_r); // 
        
        
        // validasi jarak max
        int spawn_r = 300;

        int dx = (x_structure - player_x);
        int dy = (z_structure - player_z);
        
        if (dx < 0) dx = -1 * dx;
        if (dy < 0) dy = -1 * dy;
        
        int r = spawn_r + object_r;

        // return (dx <= r) && (dy <= r); boolean
        if ((dx <= r) && (dy <= r)) {
            printf("true\n");
            strcpy(Bmaps[count].nama, type);
            count++;
        } else {
            printf("false\n");
        }
        printf("\n");
    }
    
    // Free memory
    cJSON_Delete(json);
    free(jsonData);
    


    printf("\n=== DATA LIST ===\n");
    // count = 0;

    for (int i = 0; i < count; i++) {
        printf("%d. Nama: %s\n", i, Bmaps[i].nama);
    }

    clock_t end = clock();
    double waktu = (double)(end - start) / CLOCKS_PER_SEC;  
    printf("Waktu eksekusi: %f detik\n", waktu);

    return 0;
}