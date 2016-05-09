#ifndef _SPRITE2_COLOR_H_
#define _SPRITE2_COLOR_H_

#include <stdint.h>

namespace s2
{

class Color
{
public:
	union
	{
		struct
		{
			uint8_t r;
			uint8_t g;
			uint8_t b;
			uint8_t a;
		};

		uint32_t rgba;
	};

public:
	Color();
	Color(uint32_t rgba);
	Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

}; // Color

}

#include "Color.inl"

#endif // _SPRITE2_COLOR_H_