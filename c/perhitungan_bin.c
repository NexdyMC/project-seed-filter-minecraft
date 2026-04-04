// gcc perhitungan_bin.c -o main && main
// max jarak radius perstructure 
// bagian perhitungan player dengan structures dan pemanpiln output
// versi bin
// selesai 


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <string.h>


#define MAX_DATA 10

typedef struct {
    char nama[20];
} vaidasi_data;


typedef struct {
    char type[20];
    int x;
    int z;
} Data;


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
    if (strcmp("Village", type) == 0)  object_r = 12 * 16;
    if (strcmp("Treasure", type) == 0)  object_r = 1 * 16;
    if (strcmp("Shipwreck", type) == 0)  object_r = 2 * 16;
    if (strcmp("Ruind_portal", type) == 0)  object_r = 1 * 16;
    if (strcmp("Desert", type) == 0)  object_r = 2 * 16;
    if (strcmp("Jungle", type) == 0)  object_r = 2 * 16;
    if (strcmp("Minesharft", type) == 0)  object_r = 10 * 16;
    if (strcmp("Bastion", type) == 0)  object_r = 4 * 16;
    if (strcmp("Fortress", type) == 0)  object_r = 12 * 16;
    return object_r;
}

int main() {
    int count =0 ;
    clock_t start = clock();

    
    FILE *file = fopen("s82125689760.bin", "rb");

    Data res;

    while (fread(&res, sizeof(Data), 1, file)) {
        int object_r = TypeStructureRadiusChunk(res.type);

        printf("-%s x: %d z: %d\n", res.type, res.x, res.z);
        
        int spawn_r = 200;

        int dx = (res.x - -96);
        int dy = (res.z - 64);
        
        if (dx < 0) dx = -1 * dx;
        if (dy < 0) dy = -1 * dy;
        
        int r = spawn_r + object_r;

        // return (dx <= r) && (dy <= r); boolean
        if ((dx <= r) && (dy <= r)) {
            printf("true\n");
            strcpy(Bmaps[count].nama, res.type);
            count++;
        } else {
            printf("false\n");
        }
        printf("\n");

    }
    
    for (int i = 0; i < count; i++) {
        printf("%d. Nama: %s\n", i, Bmaps[i].nama);
    }

    
    
    fclose(file);

    clock_t end = clock();
    double waktu = (double)(end - start) / CLOCKS_PER_SEC;  
    printf("Waktu eksekusi: %f detik\n", waktu);
    return 0;
}   
