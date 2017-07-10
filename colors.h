
#ifndef COLORS_H
#define COLORS_H

#include <stdint.h>

struct Color {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t alpha;
};

typedef struct Color COLOUR;

#endif
