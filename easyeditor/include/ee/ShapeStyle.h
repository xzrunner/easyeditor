#ifndef _EASYEDITOR_SHAPE_STYLE_H
#define _EASYEDITOR_SHAPE_STYLE_H

#include "Color.h"

namespace ee
{

struct ShapeStyle
{
	bool filling;
	float size;
	Colorf color;
	int line_style;

	ShapeStyle() 
		: filling(false)
		, size(2)
		, color(0,0,0,1)
		, line_style(0) 
	{}

	ShapeStyle(bool _fill, float _size, const Colorf& _color) 
		: filling(_fill)
		, size(_size)
		, color(_color)
		, line_style(0) 
	{}

	ShapeStyle(bool _fill, float _size, const Colorf& _color, int _stype) 
		: filling(_fill)
		, size(_size)
		, color(_color)
		, line_style(_stype) 
	{}

}; // ShapeStyle

}

#endif // _EASYEDITOR_SHAPE_STYLE_H