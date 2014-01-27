#ifndef D2D_SHAPE_STYLE_H
#define D2D_SHAPE_STYLE_H

#include "common/Color.h"

namespace d2d
{
	enum LineStype
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
		LineStype lineStyle;

		ShapeStyle() 
			: fill(false)
			, size(1)
			, color(0,0,0,1)
			, lineStyle(LS_DEFAULT) 
		{}
	};
}

#endif // D2D_SHAPE_STYLE_H