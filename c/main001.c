//  gcc main001.c ../cubiomes/libcubiomes.a -o main
/*
membuat file bin dan membaca file
*/

#include <stdio.h>
#include <string.h>

typedef struct {
    char type[20];
    int x;
    int z;
} Data;

int main() {
    FILE *file = fopen("data.bin", "wb");

    if (!file) {
        printf("Gagal buka file\n");
        return 1;
    }

    Data res;

    // 🔥 LOOP X dan Z
    for (int x = 1; x < 10; x++) {
        for (int z = 0; z < 10; z++) {

            strcpy(res.type, "ocean"); // ✅ BENAR
            res.x = x;
            res.z = z;

            fwrite(&res, sizeof(Data), 1, file); // ✅ BENAR
        }
    }

    fclose(file);

    printf("Data berhasil disimpan!\n");
    return 0;
}