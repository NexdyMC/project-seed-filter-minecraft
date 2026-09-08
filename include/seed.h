#ifndef SEED_H
#define SEED_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    SIGN_RANDOM = 0,
    SIGN_POSITIVE,
    SIGN_NEGATIVE
} SeedSign;

// Fungsi untuk menghasilkan seed acak berdasarkan panjang digit dan tanda
int64_t generate_seed(int length, SeedSign sign);

#endif