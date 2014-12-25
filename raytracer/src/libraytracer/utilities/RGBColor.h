#ifndef _RT_RGB_COLOR_H_
#define _RT_RGB_COLOR_H_

#include <algorithm>

namespace rt
{

class RGBColor
{
public:
	float r, g, b;

public:
	RGBColor()
		: r(0), g(0), b(0) {}
	RGBColor(float r, float g, float b)
		: r(r), g(g), b(b) {}
	RGBColor& operator = (const RGBColor& rhs); 

	RGBColor& operator += (const RGBColor& col);

	RGBColor& operator *= (const float s);
	RGBColor& operator /= (const float s);

	RGBColor operator * (const RGBColor& c) const;
	RGBColor operator * (const float a) const;
	RGBColor operator / (const float a) const;

	RGBColor Powc(float p) const;

}; // RGBColor

inline RGBColor& RGBColor::
operator = (const RGBColor& col)
{
	r = col.r; g = col.g; b = col.b;
	return *this;
}

inline RGBColor& RGBColor::
operator += (const RGBColor& col) 
{
	r += col.r; g += col.g; b += col.b;
	return *this;
}

inline RGBColor& RGBColor::
operator *= (const float s)
{
	r *= s; g *= s; b *= s;
	return *this;
}

inline RGBColor& RGBColor::
operator /= (const float s)
{
	r /= s; g /= s; b /= s;
	return *this;
}

inline RGBColor RGBColor::
operator * (const RGBColor& c) const
{
	return RGBColor(r * c.r, g * c.g, b * c.b);
}

inline RGBColor RGBColor::
operator * (const float a) const
{
	return RGBColor(r * a, g * a, b * a);
}

inline RGBColor RGBColor::
operator / (const float a) const
{
	return RGBColor(r / a, g / a, b / a);
}

//////////////////////////////////////////////////////////////////////////

inline RGBColor 
operator * (const float a, const RGBColor& c) 
{
	return RGBColor(a * c.r, a * c.g, a * c.b);
}

inline RGBColor 
operator + (const RGBColor& a, const RGBColor& b) 
{
	return RGBColor(a.r + b.r, a.g + b.g, a.b + b.b);
}

inline RGBColor ClampToColor(const RGBColor& raw_color)
{
	RGBColor c(raw_color);

	if (raw_color.r > 1.0 || raw_color.g > 1.0 || raw_color.b > 1.0) {
		c.r = 1.0; c.g = 0.0; c.b = 0.0;
	}

	return c;
}

inline RGBColor MaxToOneColor(const RGBColor& c)
{
	float max_value = std::max(c.r, std::max(c.g, c.b));

	if (max_value > 1.0)
		return c / max_value;
	else
		return c;
}

}

#endif // _RT_RGB_COLOR_H_