#ifndef DATATYPES
#define DATATYPES

#include <stdint.h>

typedef struct Colour {
	uint16_t Red;
	uint16_t Green;
	uint16_t Blue; // actually a uint_8, but everything's passed in as uint_16 anyway
}Colour;

typedef struct Position {
	uint8_t x;
	uint8_t y;
}Position;

Colour make_colour(uint16_t r, uint16_t g, uint16_t b);

#endif
