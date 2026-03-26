/*
gcc random.c -o rand &&
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

uint64_t state;

uint64_t xorshift64() {
    state ^= state << 13;
    state ^= state >> 7;
    state ^= state << 17;
    return state;
}

char* seed_random(int panjang) {
    char* result = malloc(panjang + 2);
    int index = 0;

    // tanda + / -
    if (xorshift64() % 2 == 0) {
        result[index++] = '-';
    }

    for (int i = 0; i < panjang; i++) {
        result[index++] = '0' + (xorshift64() % 10);
    }

    result[index] = '\0';
    return result;
}


int main() {
    state = time(NULL); // seed awal

    char* s = seed_random(19);
    printf("%s\n", s);

    free(s);
    return 0;
}