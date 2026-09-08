#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

typedef enum {
    SIGN_RANDOM = 0,
    SIGN_POSITIVE,
    SIGN_NEGATIVE
} SeedSign;

// Impor fungsi dari seed_random.dll
__declspec(dllimport) int64_t generate_seed(int length, SeedSign sign);

int main(int argc, char *argv[]) {
    int length = 10;          // Default panjang digit
    SeedSign sign = SIGN_RANDOM; // Default acak (positif/negatif otomatis)

    // Membaca argumen dari terminal
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--large") == 0 && i + 1 < argc) {
            length = atoi(argv[i + 1]);
            i++; // Lewati nilai angka setelah argumen --large
        } else if (strcmp(argv[i], "-p") == 0) {
            sign = SIGN_POSITIVE;
        } else if (strcmp(argv[i], "-n") == 0) {
            sign = SIGN_NEGATIVE;
        }
    }

    printf("Memproses pembuatan seed...\n");
    int64_t hasil_seed = generate_seed(length, sign);
    
    printf("%lld\n", hasil_seed);
    return 0;
}