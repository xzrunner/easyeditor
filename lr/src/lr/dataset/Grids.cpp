#include "Grids.h"

#include <ee/EE_RVG.h>
#include <ee/style_config.h>
#include <ee/Math2D.h>

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
		ee::RVG::Polyline(g.m_bird_bound, ee::LIGHT_GREY, true);
//		ee::RVG::Rect(g.m_flat_bound, ee::LIGHT_GREY_LINE);
	}
//	ee::RVG::Polyline(m_flat_bound, ee::LIGHT_GREY, true);

	for (int i = 0, n = m_debug_draw_grids.size(); i < n; ++i) {
		const Grid& g = m_grids[m_debug_draw_grids[i]];
		ee::RVG::Polyline(g.m_bird_bound, ee::BLUE, true);
	}
}

void Grids::Build(int width, int height)
{
	m_width = width;
	m_height = height;

	m_grids.clear();
	m_flat_bound.clear();

	m_col = ceil((width + BIRD_HW) / (BIRD_HW*2));
	m_row = ceil((height + BIRD_HH) / BIRD_HH);

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

std::vector<int> Grids::IntersectPolygon(const std::vector<ee::Vector>& poly) const
{
	std::vector<int> ret;

	std::vector<ee::Vector> poly_flat;
	TransVerticesToFlat(poly, poly_flat);

	int xmin, xmax, ymin, ymax;
	GetGridRegion(poly, xmin, xmax, ymin, ymax);

	for (int y = ymin; y < ymax; ++y) {
		for (int x = xmin; x < xmax; ++x) {
			int idx = y * m_col + x;
			const Grid& g = m_grids[idx];
			if (ee::Math2D::IsPolygonIntersectRect(poly_flat, g.m_flat_bound)) {
				ret.push_back(idx);
			}
		}
	}

	return ret;
}

std::vector<int> Grids::IntersectPolyline(const std::vector<ee::Vector>& path) const
{
	std::vector<int> ret;

	std::vector<ee::Vector> poly_flat;
	TransVerticesToFlat(path, poly_flat);

	int xmin, xmax, ymin, ymax;
	GetGridRegion(path, xmin, xmax, ymin, ymax);

	for (int y = ymin; y < ymax; ++y) {
		for (int x = xmin; x < xmax; ++x) {
			int idx = y * m_col + x;
			const Grid& g = m_grids[idx];
			if (ee::Math2D::IsPolylineIntersectRect(poly_flat, false, g.m_flat_bound)) {
				ret.push_back(idx);
			}
		}
	}

	return ret;
}

ee::Vector Grids::TransToBirdView(float x, float y)
{
	ee::Vector ret = ee::Math2D::RotateVector(ee::Vector(x, y), ee::PI / 4);
	ret.y *= PROJ_TRANS;
	return ret;
}

ee::Vector Grids::TransToFlatView(float x, float y)
{
	ee::Vector ret = ee::Vector(x, y);
	ret.y /= PROJ_TRANS;
	ret = ee::Math2D::RotateVector(ret, - ee::PI / 4);
	return ret;
}

void Grids::GetGridRegion(const std::vector<ee::Vector>& area, 
						  int& xmin, int& xmax, int& ymin, int& ymax) const
{
	ee::Rect r;
	for (int i = 0, n = area.size(); i < n; ++i) {
		r.Combine(area[i]);
	}

	float left = -m_width * 0.5f - BIRD_HW;
	float top = m_height * 0.5f + BIRD_HH;

	xmin = std::max(0.0f, (r.xmin-left)/(BIRD_HW*2)-1);
	xmax = std::min((float)m_col, (r.xmax-left)/(BIRD_HW*2)+2);
	ymin = std::max(0.0f, (top-r.ymax)/BIRD_HH-1);
	ymax = std::min((float)m_row, (top-r.ymin)/BIRD_HH+2);
}

void Grids::TransVerticesToFlat(const std::vector<ee::Vector>& src, std::vector<ee::Vector>& dst)
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
	m_bird_bound.push_back(ee::Vector(left+Grids::BIRD_HW, top));
	m_bird_bound.push_back(ee::Vector(left, top-Grids::BIRD_HH));
	m_bird_bound.push_back(ee::Vector(left+Grids::BIRD_HW, top-Grids::BIRD_HH*2));
	m_bird_bound.push_back(ee::Vector(left+Grids::BIRD_HW*2, top-Grids::BIRD_HH));

	ee::Vector flat[4];
	for (int i = 0; i < 4; ++i) {
		flat[i] = TransToFlatView(m_bird_bound[i].x, m_bird_bound[i].y);
		m_flat_bound.Combine(flat[i]);
	}
}

}