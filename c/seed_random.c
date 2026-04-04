#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <string.h>


int main(int argc, char *argv[]) {

    int plus_minus; 
    int seed;
    
    int data = 2;
    int digit = 19;
    int positif = 0;
    int negatif= 0;
    int keluar_loop = 0;

    srand(time(NULL));
    for (int i = 1; i < argc; i++) 
    {
        // parameter : informasi
        if ((strcmp(argv[i], "--help")) == 0|| (strcmp(argv[i], "-h")) == 0) {
            printf("Usage: seed_random [options]\n\n");
            printf("Options:\n");
            printf("\t--data:  | int |  menampilkan jumlah data yang akan ditampilkan\n");
            printf("\t--digit: | int |  menampilkan jumlah panjang number\n");
            printf("\t--help            menampilkan sebuah informasi.\n");
            printf("\t-h                menampilkan sebuah informasi.\n");
            printf("\n");
            printf("\tpositif           hanya menampilkan nilai positif\n");
            printf("\tnegatif           hanya menampilkan nilai negatif\n");
            keluar_loop =1;
            break;
        }

        // parameter : jumlah data
        if (strncmp(argv[i], "--data:", 7) == 0) {
            char *angka_str = argv[i] + 7;
            data = atoi(angka_str);
            printf("jumlah data yang akan ditampilkan %d\n", data);
        }
        
        // parameter : panjang number
        if (strncmp(argv[i], "--digit:", 8) == 0) {
            char *angka_str = argv[i] + 8;
            digit = atoi(angka_str);
            printf("panjang digit dalam angka %d\n", digit);
        }
        
        // parameter : nilai positif
        if (strcmp(argv[i], "positif") == 0) {
            positif = 1;
            printf("active bilangan positif\n");
        }

        // parameter : nilai negatif
        if (strcmp(argv[i], "negatif") == 0) {
            negatif = 1;
            printf("active bilangan negatif\n");
        }
    }
    

    for (int j = 1; j < data; j++)
    {
        if (keluar_loop == 1) {
            break;
            break;
        }

        // validasi parameter positif negatif  
        if ( positif == 1) {
            plus_minus = 1;
        } else if (negatif == 1) {
            plus_minus = 0;
        } else {
            plus_minus = rand() % 2;
        }

        // bilangan positif
        if (plus_minus == 1) {
            for (int i = 0; i < digit; i++) {
                seed = rand() % 10;
                printf("%d", seed );   
            }
        } 

        // bilangan negatif
        else if (plus_minus == 0 ) {
            printf("-");
            for (int i = 0; i < digit; i++) 
            {
                seed = rand() % 10;
                printf("%d", seed);
            }
        }
        printf("\n");
    }
    return 0;
}