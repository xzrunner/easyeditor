#include "PrimitiveDrawRVG.h"
#include "Math2D.h"

#include <rvg.h>
#include <sl_shape.h>

namespace ee
{

void PrimitiveDrawRVG::SetColor(int color)
{
	sl_shape_color(color);
}

void PrimitiveDrawRVG::Line(const Matrix& mt, const Vector& p0, const Vector& p1)
{
	Vector _p0 = Math2D::TransVector(p0, mt),
		   _p1 = Math2D::TransVector(p1, mt);
	rvg_line(_p0.x, _p0.y, _p1.x, _p1.y);
}

void PrimitiveDrawRVG::Rect(const Matrix& mt, const Vector& min, const Vector& max, bool filling)
{
	Vector _min = Math2D::TransVector(min, mt),
		   _max = Math2D::TransVector(max, mt);
	rvg_rect(min.x, min.y, max.x, max.y, filling);
}

void PrimitiveDrawRVG::Triangles(const Matrix& mt, const std::vector<Vector>& triangles)
{
// 	std::vector<Vector> coords(triangles);
// 	for (int i = 0, n = coords.size(); i < n; ++i) {
// 		coords[i] = Math2D::TransVector(coords[i], mt);
// 	}
// 	rvg_triangles(&coords[0].x, coords.size());
}

}