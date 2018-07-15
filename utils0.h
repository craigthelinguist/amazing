
#ifndef UTILS0_H
#define UTILS0_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <stdio.h>

#define LOG(out, ...) \
            do { fprintf(out, __VA_ARGS__); } while (0)

#define MAX(a, b) (a < b ? b : a)
#define MIN(a, b) (a < b ? a : b)

/* Print a number as binary, to the specified number of bits. */
void print_bits(FILE *out, int number, int precision);

bool overlapping_boxes(SDL_Rect r1, SDL_Rect r2);

#endif
