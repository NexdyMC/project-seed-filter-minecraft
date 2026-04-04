#include <stdio.h>
#include <string.h>

#define MAX_DATA 100



MapBiome Bmaps[MAX_DATA];
int count = 0;


void main() 
{
    if (count >= MAX_DATA) {
        printf("Data penuh!\n");
        return;
    }

    

    strcpy(Bmaps[count].nama, "Bastion");
    count++;
    strcpy(Bmaps[count].nama, "Fotress");
    count++;
    strcpy(Bmaps[count].nama, "Ocean");
    count++;

    printf("Data berhasil ditambahkan!\n");
    
    printf("\n=== DATA LIST ===\n");

    for (int i = 0; i < count; i++) {
        printf("%d. Nama: %s\n",
               i, Bmaps[i].nama);
    }

}