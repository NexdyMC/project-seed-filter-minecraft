// gcc render.c -o render && render

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[])
{
    clock_t start = clock();


    int p_x = 0;
    int p_y = 0;
    int p_r = 3;

    int o_x = 6; 
    int o_y = 0;
    int o_r = 2;

    int player_top  = p_y - p_r;
    int player_down = p_y + p_r;
    int player_left = p_x - p_r;
    int player_right= p_x + p_r;
    
    int object_top  = o_y - o_r;
    int object_down = o_y + o_r;
    int object_left = o_x - o_r;
    int object_right= o_x + o_r;
    

    int x1 = 0;
    int y1 = 0;
    int r1 = 3;
    
    int x2 = -6;
    int y2 = -6;
    int r2 = 2;

    int dx = (x2 - x1);
    int dy = (y2 - y1);
    
    if (x2 < 0) dx = -1 * dx;
    if (dy < 0) dy = -1 * dy;
    
    
    int r = r1 + r2;

    // return (dx <= r) && (dy <= r);
    if ((dx <= r) && (dy <= r)) {
        printf("true\n");
    } else {
        printf("false\n");

    }
    
    
    
    
    // if (
    //     (x < 100 & x > -100) & 
    //     (y < 100 & y > -100)
    // ) 
    // {
        //     printf("True");
        // } else {
            //     printf("false");
            // }
    clock_t end = clock();
    double waktu = (double)(end - start) / CLOCKS_PER_SEC;  
    printf("Waktu eksekusi: %f detik\n", waktu);

}