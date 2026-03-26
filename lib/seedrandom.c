// cd 

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

static uint64_t state = 88172645463325252ULL;

static inline uint64_t xorshift64(void) {
    state ^= state >> 12;
    state ^= state << 25;
    state ^= state >> 27;
    return state * 2685821657736338717ULL;
}

static inline void SeedRandomInit(void) {
    static int init = 0;
    if (!init) {
        state ^= (uint64_t)time(NULL);
        init = 1;
    }
}

int main(int argc, char *argv[]) {
    SeedRandomInit();


    if (strcmp(argv[1], "--help") == 0) {
        printf("Help!!\n======================\n");
        printf("random <jumlahData> <jumlahDigit>\n\n");
        printf("--data:  int | jumlah data perbaris\n");
        printf("--digit: int | jumlah kata perdigit max 19 \n\n");
        return 1;
    }
    
    
    int jumlahData = 1;
    int jumlahDigit = 1;
    
    for (int i = 1; i < argc; i++) {

        if (strncmp(argv[i], "--data:", 7) == 0) {
            jumlahData  = atoi(argv[i] + 7);
        }

        else if (strncmp(argv[i], "--digit:", 8) == 0) {
            jumlahDigit = atoi(argv[2]);
        }
    }
    

    if (jumlahDigit < 1) jumlahDigit = 18;

    uint64_t mod = 1;

    if (jumlahDigit < 19) {
        for (int j = 0; j < jumlahDigit; j++) {
            mod *= 10;
        }
    }

    for (int i = 0; i < jumlahData; i++) {

        int64_t seed = (int64_t)xorshift64();

        if (jumlahDigit < 19) {

            int sign = (seed < 0) ? -1 : 1;

            seed %= mod;
            seed *= sign;
        }

        printf("%lld\n", (long long)seed);
    }

    return 0;
}