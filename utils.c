
#include "utils0.h"

void print_bits(FILE *out, int number, int precision) {
    int i;
    for (i = 0; i < precision; i++) {
        int mask = (1 << i);
        int bit = (mask & number) >> i;
        printf("%d", bit);
    }
}