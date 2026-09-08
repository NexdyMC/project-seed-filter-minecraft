#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#ifdef _WIN32
#include <windows.h>
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

typedef enum {
    SIGN_RANDOM = 0,
    SIGN_POSITIVE,
    SIGN_NEGATIVE
} SeedSign;

static uint64_t xorshift_secure_next(uint64_t *state) {
    uint64_t x = *state;
    x ^= x << 13;
    x ^= x >> 7;
    x ^= x << 17;
    *state = x;
    return x;
}

EXPORT int64_t generate_seed(int length, SeedSign sign) {
    if (length < 1) length = 1;
    if (length > 19) length = 19;

    uint64_t seed_entropy = 0;
#ifdef _WIN32
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    seed_entropy = (uint64_t)li.QuadPart;
#else
    seed_entropy = (uint64_t)time(NULL);
#endif

    uint64_t state = seed_entropy ^ 0x9e3779b97f4a7c15ULL;
    if (state == 0) state = 0x123456789ABCDEF0ULL;

    for (int i = 0; i < 5; i++) {
        xorshift_secure_next(&state);
    }

    uint64_t raw_val = xorshift_secure_next(&state);

    int64_t min_val = (length == 1) ? 0 : (int64_t)pow(10, length - 1);
    int64_t max_val = (length == 19) ? 9223372036854775807LL : (int64_t)pow(10, length) - 1;

    int64_t range = max_val - min_val + 1;
    int64_t seed = min_val + (int64_t)(raw_val % (uint64_t)range);

    bool is_negative = false;
    if (sign == SIGN_NEGATIVE) {
        is_negative = true;
    } else if (sign == SIGN_POSITIVE) {
        is_negative = false;
    } else {
        is_negative = (xorshift_secure_next(&state) % 2 == 0);
    }

    if (is_negative && seed > 0) {
        seed = -seed;
    }

    return seed;
}