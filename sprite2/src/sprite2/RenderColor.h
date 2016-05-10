#ifndef _SPRITE2_RENDER_COLOR_H_
#define _SPRITE2_RENDER_COLOR_H_

#include "Color.h"

namespace s2
{

class RenderColor
{
public:
	Color mul, add;
	Color rmap, gmap, bmap;

public:
	RenderColor() 
		: mul(0xffffffff)
		, add(0)
		, rmap(255, 0, 0, 0)
		, gmap(0, 255, 0, 0)
		, bmap(0, 0, 255, 0)
	{}

}; // RenderColor

}

#endif // _SPRITE2_RENDER_COLOR_H_