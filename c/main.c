#include <stdio.h>
#include <string.h>

#define MAX_DATA 5

typedef struct {
    char nama[20];
} MapBiome;


MapBiome Bmaps[MAX_DATA];


int main(int argc, char *argv[]) 
{
    int count = 0;
    if (count >= MAX_DATA) {
        printf("Data penuh!\n");
        return 1;
    }

    

    strcpy(Bmaps[count].nama, "Ocean");
    count++;
    strcpy(Bmaps[count].nama, "Bastion");
    count++;
    strcpy(Bmaps[count].nama, "Fotress");
    count++;
    strcpy(Bmaps[count].nama, "Treasure");
    count++;


    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < argc; j++)
        {
            if (strcmp(Bmaps[i].nama, argv[j] ) == 0 ) {
                printf("%d done : %s \n", i, argv[j]);
            }
        }
    }

    printf("Data berhasil ditambahkan!\n");
    
    printf("\n=== DATA LIST ===\n");
    // count = 0;

    for (int i = 0; i < count; i++) {
        printf("%d. Nama: %s\n",
               i, Bmaps[i].nama);
    }
    return 0;
}