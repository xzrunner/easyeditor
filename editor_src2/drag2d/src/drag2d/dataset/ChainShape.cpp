#include "ChainShape.h"

#include <fstream>

#include "render/PrimitiveDraw.h"
#include "common/Settings.h"
#include "common/Math.h"

namespace d2d
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

ChainShape::ChainShape(const std::vector<Vector>& vertices, bool isLoop)
	: m_vertices(vertices)
{
	m_isLoop = isLoop;
	initBounding();
}

ChainShape* ChainShape::clone() const
{
	return new ChainShape(*this);
}

void ChainShape::loadFromTextFile(std::ifstream& fin)
{
	m_vertices.clear();
	size_t size;
	fin >> size;
	m_vertices.reserve(size);
	for (size_t i = 0; i < size; ++i)
	{
		Vector pos;
		fin >> pos;
		m_vertices.push_back(pos);
	}

	fin >> m_isLoop;

// 	int isLoop;
// 	fin >> isLoop;
// 	m_isLoop = (isLoop == 1 ? true : false);

	initBounding();
}

void ChainShape::storeToTextFile(std::ofstream& fout) const
{
	std::vector<Vector> fixed;
	for (size_t i = 0, n = m_vertices.size(); i < n; ++i)
	{
		if (fixed.empty() || Math::getDistance(m_vertices[i], fixed.back()) > FLT_EPSILON)
			fixed.push_back(m_vertices[i]);
	}

	fout << fixed.size() << '\n';
	for (size_t i = 0, n = fixed.size(); i < n; ++i)
		fout << fixed[i] << " ";
	fout << '\n';
	fout << m_isLoop;
	fout << '\n';
}

bool ChainShape::isContain(const Vector& pos) const
{
	Rect rect(m_rect);
	rect.xMin -= QUERY_ACCURACY;
	rect.xMax += QUERY_ACCURACY;
	rect.yMin -= QUERY_ACCURACY;
	rect.yMax += QUERY_ACCURACY;
	if (m_vertices.empty() || !Math::isPointInRect(pos, rect))
		return false;

	size_t index;
	float dis = Math::getDisPointToMultiPos(pos, m_vertices, &index);
	if (dis < QUERY_ACCURACY) return true;

	if (m_vertices.size() < 2) return false;
	for (size_t i = 0, n = m_vertices.size() - 1; i < n; ++i)
	{
		float dis = Math::getDisPointToSegment(pos, m_vertices[i], m_vertices[i + 1]);
		if (dis < QUERY_ACCURACY) 
			return true;
	}

	return false;
}

bool ChainShape::isIntersect(const Rect& rect) const
{
	if (m_vertices.empty() || !Math::isRectIntersectRect(rect, m_rect))	
		return false;

	for (size_t i = 0, n = m_vertices.size(); i < n; ++i)
	{
		if (Math::isPointInRect(m_vertices[i], rect))
			return true;
	}

	if (m_vertices.size() < 2) return false;
	for (size_t i = 0, n = m_vertices.size() - 1; i < n; ++i)
	{
		if (Math::isSegmentIntersectRect(m_vertices[i], m_vertices[i + 1], rect))
			return true;
	}

	if (m_isLoop && Math::isSegmentIntersectRect(m_vertices.front(), m_vertices.back(), rect))
		return true;

	return false;
}

void ChainShape::draw(const Colorf& color/* = Colorf(0, 0, 0)*/) const
{
	if (m_vertices.empty()) return;

	PrimitiveDraw::resetColorAndTexture();

	PrimitiveDraw::drawPolyline(m_vertices, color, m_isLoop);
	if (Settings::ctlPosSize != 0)
		PrimitiveDraw::drawCircles(m_vertices, Settings::ctlPosSize, true, 2, Colorf(0.4f, 0.8f, 0.4f));
}

void ChainShape::insertVertices(size_t index, const Vector& pos)
{
	if (index <= m_vertices.size())
		m_vertices.insert(m_vertices.begin() + index, pos);
}

void ChainShape::removeVertices(const Vector& pos)
{
	std::vector<Vector>::iterator itr = m_vertices.begin();
	for ( ; itr != m_vertices.end(); ++itr)
	{
		if (*itr == pos)
		{
			m_vertices.erase(itr);
			break;
		}
	}
}

void ChainShape::changeVertices(const Vector& from, const Vector& to)
{
	size_t index = 0;
	for (size_t n = m_vertices.size(); index < n; ++index)
	{
		if (m_vertices[index] == from)
			break;
	}

	if (index == m_vertices.size()) return;

	m_vertices[index] = to;
	if (from.x == m_rect.xMin || from.x == m_rect.xMax
		|| from.y == m_rect.yMin || from.y == m_rect.yMax)
		initBounding();
	else 
		m_rect.combine(to);
}

void ChainShape::setVertices(const std::vector<Vector>& vertices)
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

} // d2d