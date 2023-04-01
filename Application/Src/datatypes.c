#include "datatypes.h"

Colour make_colour(uint16_t r, uint16_t g, uint16_t b) {
	Colour mycol;
	mycol.Red = r;
	mycol.Green = g;
	mycol.Blue = b;

	return mycol;
}
