
#ifndef UTILS0_H
#define UTILS0_H

#include <stdio.h>

#define LOG(out, ...) \
            do { fprintf(out, __VA_ARGS__); } while (0)

/* Print a number as binary, to the specified number of bits. */
void print_bits(FILE *out, int number, int precision);

#endif
