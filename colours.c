
#include "colours.h"

Colour make_colour(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	return (Colour) {r, g, b, a};
}
