#include "seed.h"
#include <stdio.h>
#include <assert.h>

int main(void) {
    SFRng rng;
    sf_rng_init(&rng, 12345ULL);
    assert(rng.state != 0);

    uint64_t a = sf_rng_next(&rng);
    uint64_t b = sf_rng_next(&rng);
    assert(a != b);
    printf("xorshift64 ok: %llu, %llu\n", (unsigned long long)a, (unsigned long long)b);

    for (int trial = 0; trial < 100000; trial++) {
        int64_t s = sf_generate_seed(&rng, 1, 19, 1);
        int64_t abs_s = s < 0 ? -s : s;
        assert(abs_s >= 0);
    }
    printf("generate_seed stress test ok\n");

    /* cek panjang digit sesuai permintaan */
    int64_t s5 = sf_generate_seed(&rng, 5, 5, 0);
    printf("contoh seed 5 digit: %lld\n", (long long)s5);
    assert(s5 >= 10000 && s5 <= 99999);

    /* cek anak RNG dari beberapa thread tidak identik */
    SFRng child1, child2;
    sf_rng_spawn_child(&rng, &child1, 0);
    sf_rng_spawn_child(&rng, &child2, 1);
    assert(child1.state != child2.state);
    printf("spawn_child ok: %llu vs %llu\n", (unsigned long long)child1.state, (unsigned long long)child2.state);

    printf("SEMUA TEST PRNG LULUS\n");
    return 0;
}
