//  gcc main006.c -o main  && main
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define MAX_DATA 10

typedef struct {char nama[20];
} Structure;


Structure Bmaps[MAX_DATA];


int main(int argc, char *argv[]) 
{
    clock_t start = clock();
    

    int count = 0;
    if (count >= MAX_DATA) {
        printf("Data penuh!\n");
        return 1;
    }
    int query_nilai = 0;
    

    strcpy(Bmaps[count].nama, "Ocean");
    count++;
    strcpy(Bmaps[count].nama, "Bastion");
    count++;
    strcpy(Bmaps[count].nama, "Fotress");
    count++;
    strcpy(Bmaps[count].nama, "Treasure");
    count++;
    
    for (int m = 0; m < count; m++)
    {
        for (int v = 1; v < argc; v++)
        {
            if (strcmp(Bmaps[m].nama, argv[v]) == 0 ) 
            {
                // printf("sudah ada dalam data \n");    
                query_nilai++;
                break;
            } else {
                // printf("tidak ada dalam data\n");    
            }
        }
    }
    ty
    if (argc == (query_nilai + 1)) {
        printf("data barhasil dengan akurat");
    } else {
        printf("data tidak akurat");
    }

    clock_t end = clock();
    double waktu = (double)(end - start) / CLOCKS_PER_SEC;  
    printf("\nWaktu eksekusi: %f detik\n", waktu);

    return 0;
}
