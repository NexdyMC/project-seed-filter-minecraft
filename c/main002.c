// gcc main002.c cJSON.c -o main && main
// max jarak radius perstructure 
// bagian perhitungan player dengan structures dan pemanpiln output
// selesai 


#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"
#include <time.h>
#include <stdint.h>
#include <string.h>

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

int main() {
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
        int object_r;
        if (strcmp("bastion", type) == 0)  object_r = 4 * 16;
        if (strcmp("village", type) == 0)  object_r = 12 * 16;
        if (strcmp("ruind_portal", type) == 0)  object_r = 1 * 16;
        if (strcmp("fortress", type) == 0)  object_r = 10 * 16;
        if (strcmp("break", type) == 0) break;
        
        printf("- %d %s\n", i, type); // 
        printf("- %d %d\n", i, x_structure); // 
        printf("- %d %d\n", i, z_structure); // 

        // validasi jarak max
        int max_radius = 300;

        int spawn_r = 300;

        int dx = (x_structure - player_x);
        int dy = (z_structure - player_z);
        
        if (dx < 0) dx = -1 * dx;
        if (dy < 0) dy = -1 * dy;
        
        int r = spawn_r + object_r;

        // return (dx <= r) && (dy <= r);
        if ((dx <= r) && (dy <= r)) {
            printf("true\n");
        } else {
            printf("false\n");
        }
        printf("\n");
    }
    
    // Free memory
    cJSON_Delete(json);
    free(jsonData);
    
    clock_t end = clock();
    double waktu = (double)(end - start) / CLOCKS_PER_SEC;  
    printf("Waktu eksekusi: %f detik\n", waktu);

    return 0;
}