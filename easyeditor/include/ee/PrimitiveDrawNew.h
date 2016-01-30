#ifndef _EASYEDITOR_PRIMITIVE_DRAW_NEW_H_
#define _EASYEDITOR_PRIMITIVE_DRAW_NEW_H_

#include "ShapeStyle.h"

namespace ee
{

class PrimitiveDrawNew
{
public:
	static void Draw(int type, const float *coords, size_t count);
	static void Draw(int type, const float *coords, const float* colors, size_t count);

	static void SetColor(const Colorf& col);

	static void SetLineWidth(float width);
	static void SetPointSize(float size);

	static void LineStypeBegin(const LineStyle& style);
	static void LineStypeEnd(const LineStyle& style);

}; // PrimitiveDrawNew

}

#endif // _EASYEDITOR_PRIMITIVE_DRAW_NEW_H_