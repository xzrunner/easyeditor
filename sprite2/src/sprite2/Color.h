#ifndef _SPRITE2_COLOR_H_
#define _SPRITE2_COLOR_H_

#include <stdint.h>

namespace s2
{

class Color
{
public:
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;

public:
	Color();
	Color(uint32_t rgba);
	Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

	uint32_t ToABGR() const;

}; // Color

}

#include "Color.inl"

#endif // _SPRITE2_COLOR_H_