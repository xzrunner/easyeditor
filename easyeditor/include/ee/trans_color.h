#ifndef	_EASYEDITOR_TRANS_COLOR_H_
#define _EASYEDITOR_TRANS_COLOR_H_

#include "Color.h"

#include <stdint.h>

namespace ee
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

Colorf TransColor(const std::string& str, PixelType type);
Colorf TransColor(int color, PixelType type);
float TransColor(char high, char low);
int TransHex(char c);

std::string TransColor(const Colorf& col, PixelType type);
std::string TransColor(float col);
char TransHex(int v);

uint32_t color2int(const Colorf& col, PixelType type);
uint32_t color2int(float col);

}

#endif // _EASYEDITOR_TRANS_COLOR_H_