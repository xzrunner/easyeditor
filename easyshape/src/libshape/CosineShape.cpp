#include "CosineShape.h"

namespace libshape
{

#define SIMPLIFY

const float CosineShape::SAMPLING_WIDTH = 10.0f;

CosineShape::CosineShape(const CosineShape& curve)
	: ChainShape(curve)
{
	m_midPoints = curve.m_midPoints;
}

CosineShape::CosineShape(const std::vector<d2d::Vector>& vertices)
	: ChainShape(vertices, false)
{
	setMidPoints();
}

CosineShape::~CosineShape()
{
	
}

CosineShape* CosineShape::clone() const
{
	return new CosineShape(*this);	
}

void CosineShape::Translate(const d2d::Vector& offset)
{
	ChainShape::Translate(offset);
	for (int i = 0, n = m_midPoints.size(); i < n; ++i) {
		m_midPoints[i] += offset;
	}
}

void CosineShape::draw(const d2d::Matrix& mt,
					   const d2d::Colorf& color/* = Colorf(0, 0, 0)*/) const
{
	if (m_vertices.empty()) return;

	d2d::PrimitiveDraw::drawPolyline(m_midPoints, color, m_isLoop);
	if (d2d::Settings::ctlPosSize != 0) {
		d2d::PrimitiveDraw::drawCircles(m_vertices, d2d::Settings::ctlPosSize, true, 2, d2d::Colorf(0.4f, 0.8f, 0.4f));
	}
	if (d2d::Settings::ctlPosSize != 0) {
		d2d::PrimitiveDraw::drawCircles(m_midPoints, d2d::Settings::ctlPosSize * 0.5f, true, 2, d2d::Colorf(0.8f, 0.8f, 0.4f));
	}
}

d2d::IPropertySetting* CosineShape::createPropertySetting(d2d::EditPanel* editPanel)
{
	return NULL;
}

void CosineShape::insertVertices(size_t index, const d2d::Vector& pos)
{
	ChainShape::Add(index, pos);
	setMidPoints();
}

void CosineShape::removeVertices(const d2d::Vector& pos)
{
	ChainShape::Remove(pos);
	setMidPoints();
}

void CosineShape::changeVertices(const d2d::Vector& from, const d2d::Vector& to)
{
	ChainShape::Change(from, to);
	setMidPoints();
}

void CosineShape::refresh()
{
//	createBody();
}

void CosineShape::setMidPoints()
{
	m_midPoints.clear();

	if (m_vertices.size() <= 1) return;

	std::vector<d2d::Vector> smooth;
	d2d::CosineSmooth::implement(m_vertices, SAMPLING_WIDTH, smooth);

#ifdef SIMPLIFY
	d2d::DouglasPeucker::implement(smooth, 0.75f, m_midPoints);
#endif
}

} // d2d