#ifndef _EASYEDITOR_SHAPE_STYLE_H
#define _EASYEDITOR_SHAPE_STYLE_H

#include "Color.h"

namespace ee
{

enum LineStyle
{
	LS_DEFAULT = 0,
	LS_DOT,
	LS_DASH,
	LS_DOT_DASH
};

struct ShapeStyle
{
	bool fill;
	float size;
	Colorf color;
	LineStyle lineStyle;

	ShapeStyle() 
		: fill(false)
		, size(2)
		, color(0,0,0,1)
		, lineStyle(LS_DEFAULT) 
	{}

	ShapeStyle(bool _fill, const Colorf& _color) 
		: fill(_fill)
		, size(2)
		, color(_color)
		, lineStyle(LS_DEFAULT) 
	{}

	ShapeStyle(bool _fill, float _size, const Colorf& _color, LineStyle _stype) 
		: fill(_fill)
		, size(_size)
		, color(_color)
		, lineStyle(_stype) 
	{}

}; // ShapeStyle

}

#endif // _EASYEDITOR_SHAPE_STYLE_H