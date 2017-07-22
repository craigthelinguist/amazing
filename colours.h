
#ifndef COLORS_H
#define COLORS_H

#include <stdint.h>

struct Colour {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t alpha;
};

typedef struct Colour Colour;

Colour make_colour(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

#endif
