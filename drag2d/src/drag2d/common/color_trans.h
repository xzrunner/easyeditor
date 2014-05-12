#ifndef	D2D_COLOR_TRANS_H
#define D2D_COLOR_TRANS_H

#include "Color.h"

namespace d2d
{
	// text		RGBA
	// add		ARGB
	// multi	BGRA
	enum PixelType
	{
		PT_RGBA = 0,
		PT_ARGB,
		PT_BGRA
	};

	Colorf transColor(const std::string& str, PixelType type);
	Colorf transColor(int color, PixelType type);
	float transColor(char high, char low);
	int transHex(char c);

	std::string transColor(const Colorf& col, PixelType type);
	std::string transColor(float col);
	char transHex(int v);
}

#endif // D2D_COLOR_TRANS_H