#include "Grids.h"

namespace lr
{

const float Grids::BIRD_HW = 35;
const float Grids::BIRD_HH = 26.272f;
const float Grids::PROJ_TRANS = Grids::BIRD_HH / Grids::BIRD_HW;
const float Grids::EDGE = sqrt(Grids::BIRD_HW * Grids::BIRD_HW + Grids::BIRD_HH * Grids::BIRD_HH);

// const float Grids::PROJ_TRANS = 0.75f;
// const float Grids::BIRD_HW = Grids::EDGE * sqrt(2.0f) * 0.5f;
// const float Grids::BIRD_HH = Grids::BIRD_HW * Grids::PROJ_TRANS;

void Grids::Draw() const
{
	for (int i = 0, n = m_grids.size(); i < n; ++i)
	{
		const Grid& g = m_grids[i];
		d2d::PrimitiveDraw::drawPolyline(g.m_bird_bound, d2d::LIGHT_GREY, true);
//		d2d::PrimitiveDraw::rect(g.m_flat_bound, d2d::LIGHT_GREY_LINE);
	}
//	d2d::PrimitiveDraw::drawPolyline(m_flat_bound, d2d::LIGHT_GREY, true);

	for (int i = 0, n = m_debug_draw_grids.size(); i < n; ++i) {
		const Grid& g = m_grids[m_debug_draw_grids[i]];
		d2d::PrimitiveDraw::drawPolyline(g.m_bird_bound, d2d::BLUE, true);
	}
}

void Grids::Build(int width, int height)
{
	m_width = width;
	m_height = height;

	m_grids.clear();
	m_flat_bound.clear();

	m_col = std::ceil((width + BIRD_HW) / (BIRD_HW*2));
	m_row = std::ceil((height + BIRD_HH) / BIRD_HH);

	float left = -width * 0.5f,
		right = -left,
		top = height * 0.5f,
		bottom = -top;
	float cx = left, cy = top + BIRD_HH;

	for (int y = 0; y < m_row; ++y) {
		cx = (y % 2) == 0 ? left : left - BIRD_HW;
		for (int x = 0; x < m_col; ++x) {
			m_grids.push_back(Grid(cx, cy));
			cx += BIRD_HW*2;
		}
		cy -= BIRD_HH;
	}

	m_flat_bound.push_back(TransToFlatView(-width*0.5f, height*0.5f));
	m_flat_bound.push_back(TransToFlatView( width*0.5f, height*0.5f));
	m_flat_bound.push_back(TransToFlatView( width*0.5f, -height*0.5f));
	m_flat_bound.push_back(TransToFlatView(-width*0.5f, -height*0.5f));
}

std::vector<int> Grids::IntersectPolygon(const std::vector<d2d::Vector>& poly) const
{
	std::vector<int> ret;

	std::vector<d2d::Vector> poly_flat;
	TransVerticesToFlat(poly, poly_flat);

	int xmin, xmax, ymin, ymax;
	GetGridRegion(poly, xmin, xmax, ymin, ymax);

	for (int y = ymin; y < ymax; ++y) {
		for (int x = xmin; x < xmax; ++x) {
			int idx = y * m_col + x;
			const Grid& g = m_grids[idx];
			if (d2d::Math::IsPolygonIntersectRect(poly_flat, g.m_flat_bound)) {
				ret.push_back(idx);
			}
		}
	}

	return ret;
}

std::vector<int> Grids::IntersectPolyline(const std::vector<d2d::Vector>& path) const
{
	std::vector<int> ret;

	std::vector<d2d::Vector> poly_flat;
	TransVerticesToFlat(path, poly_flat);

	int xmin, xmax, ymin, ymax;
	GetGridRegion(path, xmin, xmax, ymin, ymax);

	for (int y = ymin; y < ymax; ++y) {
		for (int x = xmin; x < xmax; ++x) {
			int idx = y * m_col + x;
			const Grid& g = m_grids[idx];
			if (d2d::Math::isPolylineIntersectRect(poly_flat, false, g.m_flat_bound)) {
				ret.push_back(idx);
			}
		}
	}

	return ret;
}

d2d::Vector Grids::TransToBirdView(float x, float y)
{
	d2d::Vector ret = d2d::Math::rotateVector(d2d::Vector(x, y), d2d::PI / 4);
	ret.y *= PROJ_TRANS;
	return ret;
}

d2d::Vector Grids::TransToFlatView(float x, float y)
{
	d2d::Vector ret = d2d::Vector(x, y);
	ret.y /= PROJ_TRANS;
	ret = d2d::Math::rotateVector(ret, - d2d::PI / 4);
	return ret;
}

void Grids::GetGridRegion(const std::vector<d2d::Vector>& area, 
						  int& xmin, int& xmax, int& ymin, int& ymax) const
{
	d2d::Rect r;
	for (int i = 0, n = area.size(); i < n; ++i) {
		r.combine(area[i]);
	}

	float left = -m_width * 0.5f - BIRD_HW;
	float top = m_height * 0.5f + BIRD_HH;

	xmin = std::max(0.0f, (r.xMin-left)/(BIRD_HW*2)-1);
	xmax = std::min((float)m_col, (r.xMax-left)/(BIRD_HW*2)+2);
	ymin = std::max(0.0f, (top-r.yMax)/BIRD_HH-1);
	ymax = std::min((float)m_row, (top-r.yMin)/BIRD_HH+2);
}

void Grids::TransVerticesToFlat(const std::vector<d2d::Vector>& src, std::vector<d2d::Vector>& dst)
{
	dst.resize(src.size());
	for (int i = 0, n = src.size(); i < n; ++i) {
		dst[i] = TransToFlatView(src[i].x, src[i].y);
	}
}

//////////////////////////////////////////////////////////////////////////
// class Grids::Grid
//////////////////////////////////////////////////////////////////////////

Grids::Grid::Grid(float left, float top)
{
	m_bird_bound.push_back(d2d::Vector(left+Grids::BIRD_HW, top));
	m_bird_bound.push_back(d2d::Vector(left, top-Grids::BIRD_HH));
	m_bird_bound.push_back(d2d::Vector(left+Grids::BIRD_HW, top-Grids::BIRD_HH*2));
	m_bird_bound.push_back(d2d::Vector(left+Grids::BIRD_HW*2, top-Grids::BIRD_HH));

	d2d::Vector flat[4];
	for (int i = 0; i < 4; ++i) {
		flat[i] = TransToFlatView(m_bird_bound[i].x, m_bird_bound[i].y);
		m_flat_bound.combine(flat[i]);
	}
}

}