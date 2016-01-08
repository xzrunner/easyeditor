#include "PrimitiveDrawRVG.h"

#include "common/Math.h"

#include <rvg.h>

namespace d2d
{

void PrimitiveDrawRVG::SetColor(int color)
{
	rvg_shader_color(color);
}

void PrimitiveDrawRVG::Line(const Matrix& mt, const d2d::Vector& p0, const d2d::Vector& p1)
{
	Vector _p0 = Math::transVector(p0, mt),
		   _p1 = Math::transVector(p1, mt);
	rvg_line(_p0.x, _p0.y, _p1.x, _p1.y);
}

void PrimitiveDrawRVG::Rect(const Matrix& mt, const Vector& min, const Vector& max, bool filling)
{
	Vector _min = Math::transVector(min, mt),
		   _max = Math::transVector(max, mt);
	rvg_rect(min.x, min.y, max.x, max.y, filling);
}

void PrimitiveDrawRVG::Triangles(const Matrix& mt, const std::vector<Vector>& triangles)
{
	std::vector<Vector> coords(triangles);
	for (int i = 0, n = coords.size(); i < n; ++i) {
		coords[i] = Math::transVector(coords[i], mt);
	}
	rvg_triangles(&coords[0].x, coords.size());
}

}