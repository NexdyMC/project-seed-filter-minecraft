#include "validate.h"
#include <stdio.h>
#include <assert.h>

int main(void) {
    assert(sf_struct_radius(SF_VILLAGE) == 192);
    assert(sf_struct_radius(SF_TREASURE) == 16);
    assert(sf_struct_radius(SF_FORTRESS) == 192);
    assert(sf_struct_radius(SF_MINESHAFT) == 160);
    printf("tabel radius ok\n");

    int dx, dz, th;
    /* player di (0,0), struktur village di (100,50), radiusPlayer=16 */
    int r = sf_validate_distance(0, 0, 100, 50, 16, sf_struct_radius(SF_VILLAGE), &dx, &dz, &th);
    printf("dx=%d dz=%d th=%d passed=%d\n", dx, dz, th, r);
    assert(dx == 100 && dz == 50 && th == 208 && r == 1);

    /* struktur jauh -> gagal */
    r = sf_validate_distance(0, 0, 5000, 0, 16, sf_struct_radius(SF_TREASURE), &dx, &dz, &th);
    assert(r == 0);
    printf("kasus jauh gagal dengan benar\n");

    /* uji koordinat negatif */
    r = sf_validate_distance(-200, -200, -150, -250, 16, sf_struct_radius(SF_SHIPWRECK), &dx, &dz, &th);
    printf("dx=%d dz=%d th=%d passed=%d\n", dx, dz, th, r);
    assert(dx == 50 && dz == 50);

    printf("SEMUA TEST VALIDATE LULUS\n");
    return 0;
}
