// code from http://axonflux.com/handy-rgb-to-hsl-and-rgb-to-hsv-color-model-c

#include "convert_color.h"

#include "Math2D.h"

namespace ee
{

static inline float hue2rgb(float p, float q, float t) 
{
	if (t < 0) t += 1;
	if (t > 1) t -= 1;
	if (t < 1.0f / 6.0f) return p + (q - p) * 6 * t;
	if (t < 1.0f / 2.0f) return q;
	if (t < 2.0f / 3.0f) return p + (q - p) * (2.0f / 3.0f - t) * 6;
	return p;
}

void hsl2rgb(float h, float s, float l, float& r, float& g, float& b)
{
	if (s == 0) {
		r = g = b = l;
	} else {
		float q = l < 0.5 ? l * (1 + s) : l + s - l * s;
		float p = 2 * l - q;
		r = hue2rgb(p, q, h + 1.0f / 3.0f);
		g = hue2rgb(p, q, h);
		b = hue2rgb(p, q, h - 1.0f / 3.0f);
	}
}

void rgb2hsl(float r, float g, float b, float& h, float& s, float& l)
{
	float max = std::max(std::max(r, g), b),
		  min = std::min(std::min(r, g), b);
	h = s = l = (max + min) / 2;
	if (max == min) {
		h = s = 0;
	} else {
		float d = max - min;
		s = l > 0.5 ? d / (2 - max - min) : d / (max + min);
		if (max == r) {
			h = (g - b) / d + (g < b ? 6 : 0);
		} else if (max == g) {
			h = (b - r) / d + 2;
		} else if (max == b) {
			h = (r - g) / d + 4;
		}
		h /= 6;
	}
}

}