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
	explicit Color(int r, int g, int b, int a = 255);

	bool operator != (const Color& col) const;
	bool operator == (const Color& col) const;

	Color operator * (const Color& col) const;
	Color operator + (const Color& col) const;
	Color MapMul(const Color& rmap, const Color& gmap, const Color& bmap) const;

	void FromFloat(float r, float g, float b, float a = 1.0f);

	uint32_t ToABGR() const;
	void FromABGR(uint32_t abgr);

	// todo: rm
	uint32_t ToRGBA() const;
	void FromRGBA(uint32_t rgba);
	
}; // Color

}

#include "Color.inl"

#endif // _SPRITE2_COLOR_H_