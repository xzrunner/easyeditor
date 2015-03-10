#include "color_convert.h"

#include "common/Math.h"

namespace d2d
{

Colori hsl2rgb(float h, float s, float l)
{
	h /= 255 ;
	s /= 255 ;
	l /= 255 ;

	float r, g, b ;
	r = 0.0 + std::max (0.0, std::min (1.0, (0.5 + cos (d2d::PI / 180.0 * (00.0 + h * 360.0))))) ;
	g = 1.0 - std::max (0.0, std::min (1.0, (0.5 + cos (d2d::PI / 180.0 * (60.0 + h * 360.0))))) ;
	b = 1.0 - std::max (0.0, std::min (1.0, (0.5 + cos (d2d::PI / 180.0 * (60.0 - h * 360.0))))) ;

	r = l + (r - l) * s ;
	g = l + (g - l) * s ;
	b = l + (b - l) * s ;

	r += (l - 0.5f) * 2.0f * (l < 0.5f ? r : (1.0f - r)) ;
	g += (l - 0.5f) * 2.0f * (l < 0.5f ? g : (1.0f - g)) ;
	b += (l - 0.5f) * 2.0f * (l < 0.5f ? b : (1.0f - b)) ;

	return Colori(r * 255, g * 255, b * 255);
}

Colori rgb2hsl(float r, float g, float b)
{
	r /= 255 ;
	g /= 255 ;
	b /= 255 ;

	float fMax = std::max (r, std::max (g, b)) ;
	float fMin = std::min (r, std::min (g, b)) ;

	float h = 0 , s = 1.0f, l = 0.5f, r_dist, g_dist, b_dist;
	l = (fMax + fMin) / 2 ;
	if (fMax - fMin <= 0.00001)
	{
		h = 0 ;
		s = 0 ;
	}
	else
	{
		s = (fMax - fMin) / ((l < 0.5) ? (fMax + fMin) : (2 - fMax - fMin)) ;
		r_dist = (fMax - r) / (fMax - fMin) ;
		g_dist = (fMax - g) / (fMax - fMin) ;
		b_dist = (fMax - b) / (fMax - fMin) ;
		if (r == fMax) h = b_dist - g_dist ;
		else
			if (g == fMax) h = 2 + r_dist - b_dist ;
			else
				if (b == fMax) h = 4 + g_dist - r_dist ;
		h *= 60 ;
		if (h < 0) h += 360 ;
	}

	return Colori(h * 255 / 360.0, s * 255, l * 255);
}

}