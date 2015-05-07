#include "Grids.h"

namespace lr
{

const float Grids::PROJ_TRANS = 0.75f;
const float Grids::BIRD_HW = Grids::EDGE * sqrt(2.0f) * 0.5f;
const float Grids::BIRD_HH = Grids::BIRD_HW * Grids::PROJ_TRANS;

void Grids::Draw() const
{
	for (int i = 0, n = m_grids.size(); i < n; ++i)
	{
		const Grid& g = m_grids[i];
		d2d::PrimitiveDraw::drawPolyline(g.m_bird_bound, d2d::LIGHT_GREY, true);
//		d2d::PrimitiveDraw::rect(g.m_flat_bound, d2d::LIGHT_GREY_LINE);
	}
//	d2d::PrimitiveDraw::drawPolyline(m_flat_bound, d2d::LIGHT_GREY, true);
}

void Grids::Build(int width, int height)
{
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