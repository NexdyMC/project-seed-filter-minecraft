// gcc main006.c -o main6
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <string.h>

int main()
{
    
    srand(time(NULL));


    int total = 0;
    int seed;
    // Gunakan < 5 agar loop berjalan saat total 0,1,2,3,4
    while (total < 5) { 
        seed = rand() % 10;
        printf("%d\n", seed);
        
        
        if (seed == 5) {
            printf("done : %d\n", seed);
            total++;
        }
        // for (int j = 1; j < 20; j++) { 
                           
        // }
        if (total == 5) break; // Keluar dari loop for
    }
    printf("Proses selesai. Total: %d", total);



    return 0;
}