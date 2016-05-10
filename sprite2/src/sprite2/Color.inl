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
Color::Color(int r, int g, int b, int a)
	: r(r)
	, g(g)
	, b(b)
	, a(a) 
{}

inline
bool Color::operator != (const Color& col) const
{
	return !(this->operator == (col));
}

inline
bool Color::operator == (const Color& col) const
{
	return r == col. r && g == col. g && b == col. b && a == col.a ;
}

inline
Color Color::operator * (const Color& col) const
{
	Color ret;
	ret.r = r * col.r / 255;
	ret.g = g * col.g / 255;
	ret.b = b * col.b / 255;
	ret.a = a * col.a / 255;
	return ret;
}

static inline 
unsigned int clamp(unsigned int c) {
	return ((c) > 255 ? 255 : (c));
}

inline
Color Color::operator + (const Color& col) const
{
	Color ret;
	ret.r = clamp(r + col.r);
	ret.g = clamp(g + col.g);
	ret.b = clamp(b + col.b);
	ret.a = clamp(a + col.a);
	return ret;
}

inline
Color Color::MapMul(const Color& rmap, const Color& gmap, const Color& bmap) const
{
	Color ret;
	ret.r = (r * rmap.r + g * gmap.r + b * bmap.r) / 255;
	ret.g = (r * rmap.g + g * gmap.g + b * bmap.g) / 255;
	ret.b = (r * rmap.b + g * gmap.b + b * bmap.b) / 255;
	return ret;
}

inline
void Color::FromFloat(float r, float g, float b, float a)
{
	this->r = uint8_t(r * 255);
	this->g = uint8_t(g * 255);
	this->b = uint8_t(b * 255);
	this->a = uint8_t(a * 255);
}

inline
uint32_t Color::ToABGR() const
{
	return (a << 24 ) | (b << 16) | (g << 8) | r;
}

inline
void Color::FromABGR(uint32_t abgr)
{
	a = (abgr >> 24) & 0xff;
	b = (abgr >> 16) & 0xff;
	g = (abgr >> 8)  & 0xff;
	r = abgr         & 0xff;
}

inline
uint32_t Color::ToRGBA() const
{
	return (r << 24 ) | (g << 16) | (b << 8) | a;
}

inline
void Color::FromRGBA(uint32_t rgba)
{
	r = (rgba >> 24) & 0xff;
	g = (rgba >> 16) & 0xff;
	b = (rgba >> 8)  & 0xff;
	a = rgba         & 0xff;
}

}

#endif // _SPRITE2_COLOR_INL_