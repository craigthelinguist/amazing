
#include <math.h>
#include <stdlib.h>
#include "utils0.h"

void print_bits(FILE *out, int number, int precision) {
    int i;
    for (i = 0; i < precision; i++) {
        int mask = (1 << i);
        int bit = (mask & number) >> i;
        printf("%d", bit);
    }
}

bool overlapping_boxes(SDL_Rect r1, SDL_Rect r2) {
    return r1.x + r1.w >= r2.x
            && r1.y + r1.h >= r2.y
            && r1.x <= r2.x + r2.w
            && r1.y <= r2.y + r2.h;
}

