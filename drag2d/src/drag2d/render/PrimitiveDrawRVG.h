#ifndef _DRAG2D_PRIMITIVE_DRAW_RVG_H_
#define _DRAG2D_PRIMITIVE_DRAW_RVG_H_

#include <vector>

namespace d2d
{

class Matrix;
class Vector;

class PrimitiveDrawRVG
{
public:
	static void SetColor(int color);

	static void Line(const Matrix& mt, const Vector& p0, const Vector& p1);

	static void Rect(const Matrix& mt, const Vector& min, const Vector& max, bool filling);

	static void Triangles(const Matrix& mt, const std::vector<Vector>& triangles);

}; // PrimitiveDrawRVG

}

#endif // _DRAG2D_PRIMITIVE_DRAW_RVG_H_