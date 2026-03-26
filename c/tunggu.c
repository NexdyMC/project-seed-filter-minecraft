#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

int main() {

    time_t start_time = time(NULL);

    while(1) {

        FILE *f = fopen("data.json", "r");

        if(f != NULL) {

            char buffer[512];
            fread(buffer, 1, sizeof(buffer), f);
            fclose(f);

            // if(strstr(buffer, ".json") != NULL) {

            //     printf("File json ditemukan, menjalankan validator...\n");
            //     system("start validator.exe");
            
            // } else {

                // printf("File ada tapi belum ada nama json...\n");
                // }
                
            printf("File ada nama json...\n");
            break;
        } else {

            printf("Menunggu file json...\n");
        }

        if(difftime(time(NULL), start_time) > 300) {    

            printf("Timeout 5 menit. Program dihentikan.\n");
            break;
        }

        Sleep(1000); // cek setiap 1 detik
    }

    return 0;
};