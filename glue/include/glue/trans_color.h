#ifndef	_GLUE_TRANS_COLOR_H_
#define _GLUE_TRANS_COLOR_H_

#include <sprite2/Color.h>

#include <string>

namespace glue
{

// text		RGBA	old

// text		ARGB
// add		ABGR
// multi	BGRA
enum PixelType
{
	PT_RGBA = 0,
	PT_ARGB,
	PT_ABGR,
	PT_BGRA
};

s2::Color str2color(const std::string& str, PixelType type);
std::string color2str(const s2::Color& col, PixelType type);

s2::Color int2color(uint32_t i, PixelType type);
uint32_t color2int(const s2::Color& col, PixelType type);

}

#endif // _GLUE_TRANS_COLOR_H_