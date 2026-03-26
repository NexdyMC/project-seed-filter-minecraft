//  gcc main001.c ../cubiomes/libcubiomes.a -o main
/*
membuat file bin dan membaca file
*/

#include <stdio.h>

typedef struct {
    char type[20];
    int x;
    int z;
} Data;

int main() {
    FILE *file = fopen("data.bin", "rb");

    Data res;

    while (fread(&res, sizeof(Data), 1, file)) {
        printf("type=%s x=%d z=%d\n", res.type, res.x, res.z);
    }

    fclose(file);
}   