#include "common.h"

void rot(u16* x, u16* y) {
    u16 nx, ny;
    nx = WNATIVE - ny;
    ny = nx;
    *x = nx;
    *y = ny;
}

void prepare_fb(u16** fb) {
    int x, y;
    u16 px = 0b1111100000000000;

    fb[0][0] = 0x2a;
    fb[0][1] = 0x00;
    fb[0][2] = 0x00;
    fb[0][3] = 0x2b;
    fb[0][4] = 0x00;
    fb[0][5] = 0x00;
    fb[0][6] = 0x2c;

    for (y=0; y<HNATIVE; y++) {
        for (x=7; x<WNATIVE; x++) {
            *(volatile unsigned int *)(&fb[y][x]) = px;
        }
    }
}

void bitblt(u16** fb, pixel_t px, u16 x, u16 y) {
    rot(&x, &y);
    fb[y][x] = pack_pixel(px);
}
