#ifndef _SPRITE2_COLOR_INL_
#define _SPRITE2_COLOR_INL_

namespace s2
{

inline
Color::Color() 
	: rgba(0) 
{}

inline
Color::Color(uint32_t rgba) 
	: rgba(rgba) 
{}

inline
Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	: r(r)
	, g(g)
	, b(b)
	, a(a) 
{}

}

#endif // _SPRITE2_COLOR_INL_