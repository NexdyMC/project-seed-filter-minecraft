#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

void urutkan(int data[]) 
{
    
    int n = sizeof(data) / sizeof(data[0]);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (data[j] > data[j + 1]) {
                int temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
            }
        }
    }
    
    printf("Hasil sorting:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", data[i]);
    }   
}


int main(int argc, char *argv[])
{
    // int data[] = {5, 2, 9, 1};
    // int n = sizeof(data) / sizeof(data[0]);

    // // Bubble Sort
    // for (int i = 0; i < n; i++) {
    //     for (int j = 0; j < n - i - 1; j++) {
    //         if (data[j] > data[j + 1]) {
    //             int temp = data[j];
    //             data[j] = data[j + 1];
    //             data[j + 1] = temp;
    //         }
    //     }
    // }
    // printf("Hasil sorting:\n");
    // for (int i = 0; i < n; i++) {
    //     printf("%d ", data[i]);
    // }  
    urutkan([5, 2, 9, 1]);
    return 0;
}