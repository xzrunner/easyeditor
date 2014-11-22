#include "ChainShape.h"
#include "ChainPropertySetting.h"

namespace libshape
{

ChainShape::ChainShape()
{
	m_isLoop = false;
}

ChainShape::ChainShape(const ChainShape& chain)
{
	copy(chain.m_vertices.begin(), chain.m_vertices.end(), back_inserter(m_vertices));
	m_isLoop = chain.m_isLoop;
	m_rect = chain.m_rect;
}

ChainShape::ChainShape(const std::vector<d2d::Vector>& vertices, bool isLoop)
	: m_vertices(vertices)
{
	m_isLoop = isLoop;
	initBounding();
}

ChainShape* ChainShape::clone() const
{
	return new ChainShape(*this);
}

bool ChainShape::isContain(const d2d::Vector& pos) const
{
	d2d::Rect rect(m_rect);
	rect.xMin -= QUERY_ACCURACY;
	rect.xMax += QUERY_ACCURACY;
	rect.yMin -= QUERY_ACCURACY;
	rect.yMax += QUERY_ACCURACY;
	if (m_vertices.empty() || !d2d::Math::isPointInRect(pos, rect))
		return false;

	size_t index;
	float dis = d2d::Math::getDisPointToMultiPos(pos, m_vertices, &index);
	if (dis < QUERY_ACCURACY) return true;

	if (m_vertices.size() < 2) return false;
	for (size_t i = 0, n = m_vertices.size() - 1; i < n; ++i)
	{
		float dis = d2d::Math::getDisPointToSegment(pos, m_vertices[i], m_vertices[i + 1]);
		if (dis < QUERY_ACCURACY) 
			return true;
	}

	return false;
}

bool ChainShape::isIntersect(const d2d::Rect& rect) const
{
	if (m_vertices.empty() || !d2d::Math::isRectIntersectRect(rect, m_rect))	
		return false;

	for (size_t i = 0, n = m_vertices.size(); i < n; ++i)
	{
		if (d2d::Math::isPointInRect(m_vertices[i], rect))
			return true;
	}

	if (m_vertices.size() < 2) return false;
	for (size_t i = 0, n = m_vertices.size() - 1; i < n; ++i)
	{
		if (d2d::Math::isSegmentIntersectRect(m_vertices[i], m_vertices[i + 1], rect))
			return true;
	}

	if (m_isLoop && d2d::Math::isSegmentIntersectRect(m_vertices.front(), m_vertices.back(), rect))
		return true;

	return false;
}

void ChainShape::draw(const d2d::Colorf& color/* = Colorf(0, 0, 0)*/) const
{
	if (m_vertices.empty()) return;

	d2d::PrimitiveDraw::drawPolyline(m_vertices, color, m_isLoop);
	if (d2d::Settings::ctlPosSize != 0) {
		d2d::PrimitiveDraw::drawCircles(m_vertices, d2d::Settings::ctlPosSize, true, 2, d2d::Colorf(0.4f, 0.8f, 0.4f));
	}
}

d2d::IPropertySetting* ChainShape::createPropertySetting(d2d::EditPanel* editPanel)
{
	return new ChainPropertySetting(editPanel, this);
}

void ChainShape::insertVertices(size_t index, const d2d::Vector& pos)
{
	if (index <= m_vertices.size())
		m_vertices.insert(m_vertices.begin() + index, pos);
}

void ChainShape::removeVertices(const d2d::Vector& pos)
{
	std::vector<d2d::Vector>::iterator itr = m_vertices.begin();
	for ( ; itr != m_vertices.end(); ++itr) {
		if (*itr == pos) {
			m_vertices.erase(itr);
			break;
		}
	}
}

void ChainShape::changeVertices(const d2d::Vector& from, const d2d::Vector& to)
{
	size_t index = 0;
	for (size_t n = m_vertices.size(); index < n; ++index)
	{
		if (m_vertices[index] == from)
			break;
	}

	if (index == m_vertices.size()) return;

	m_vertices[index] = to;
	if (from.x == m_rect.xMin || from.x == m_rect.xMax || 
		from.y == m_rect.yMin || from.y == m_rect.yMax) {
		initBounding();
	} else {
		m_rect.combine(to);
	}
}

void ChainShape::setVertices(const std::vector<d2d::Vector>& vertices)
{
	m_vertices = vertices;
	initBounding();
}

void ChainShape::initBounding()
{
	m_rect.makeInfinite();
	for (size_t i = 0, n = m_vertices.size(); i < n; ++i)
		m_rect.combine(m_vertices[i]);
}

}