#ifndef _SPRITE2_COLOR_INL_
#define _SPRITE2_COLOR_INL_

namespace s2
{

inline
Color::Color() 
	: r(0)
	, g(0)
	, b(0)
	, a(0)
{}

inline
Color::Color(uint32_t rgba) 
{
	r = (rgba >> 24) & 0xff;
	g = (rgba >> 16) & 0xff;
	b = (rgba >>  8) & 0xff;
	a = rgba         & 0xff;
}

inline
Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	: r(r)
	, g(g)
	, b(b)
	, a(a) 
{}

inline
uint32_t Color::ToABGR() const
{
	return (a << 24 ) | (b << 16) | (g << 8) | r;
}

}

#endif // _SPRITE2_COLOR_INL_