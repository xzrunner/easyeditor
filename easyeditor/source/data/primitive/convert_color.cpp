// copy from http://www.geekymonkey.com/Programming/CSharp/RGB2HSL_HSL2RGB.htm

#include "convert_color.h"

#include "Math2D.h"

namespace ee
{

Colorf hsl2rgb(float h, float s, float l)
{
	float r, g, b;
	r = g = b = l;		// default to gray

	float v = (l <= 0.5f) ? (l * (1.0f + s)) : (l + s - l * s);
	if (v <= 0) {
		return Colorf(r, g, b);
	}

	float m = l + l - v;
	float sv = (v - m ) / v;
	h *= 6.0f;
	int sextant = (int)h;
	float fract = h - sextant;
	float vsf = v * sv * fract;
	float mid1 = m + vsf;
	float mid2 = v - vsf;
	switch (sextant)
	{
	case 0:
		r = v;
		g = mid1;
		b = m;
		break;
	case 1:
		r = mid2;
		g = v;
		b = m;
		break;
	case 2:
		r = m;
		g = v;
		b = mid1;
		break;
	case 3:
		r = m;
		g = mid2;
		b = v;
		break;
	case 4:
		r = mid1;
		g = m;
		b = v;
		break;
	case 5:
		r = v;
		g = m;
		b = mid2;
		break;
	}

	return Colorf(r, g, b);
}

Colorf rgb2hsl(float r, float g, float b)
{
	float h, s, l;
	h = s = l = 0;		// default to black

	float v = std::max(std::max(r, g), b);
	float m = std::min(std::min(r, g), b);
	l = (m + v) / 2.0f;
	if (l <= 0.0) {
		return Colorf(h, s, l);
	}
	float vm = v - m;
	s = vm;
	if (s > 0.0f) {
		s /= (l <= 0.5f) ? (v + m ) : (2.0f - v - m) ;
	} else {
		return Colorf(h, s, l);
	}
	float r2 = (v - r) / vm;
	float g2 = (v - g) / vm;
	float b2 = (v - b) / vm;
	if (r == v) {
		h = (g == m ? 5.0f + b2 : 1.0f - g2);
	} else if (g == v) {
		h = (b == m ? 1.0f + r2 : 3.0f - b2);
	} else {
		h = (r == m ? 3.0f + g2 : 5.0f - r2);
	}
	h /= 6.0;

	return Colorf(h, s, l);
}

}