#ifndef _RT_RGB_COLOR_H_
#define _RT_RGB_COLOR_H_

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

	RGBColor operator * (const RGBColor& c) const;
	RGBColor operator * (const float a) const;

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
	r += col.r;
	g += col.g;
	b += col.b;
	return (*this);		
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

inline RGBColor 
operator * (const float a, const RGBColor& c) 
{
	return RGBColor(a * c.r, a * c.g, a * c.b);
}

}

#endif // _RT_RGB_COLOR_H_