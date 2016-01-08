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
	InitBounding();
}

ChainShape* ChainShape::Clone() const
{
	return new ChainShape(*this);
}

bool ChainShape::IsContain(const d2d::Vector& pos) const
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

bool ChainShape::IsIntersect(const d2d::Rect& rect) const
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

void ChainShape::Translate(const d2d::Vector& offset)
{
	for (int i = 0, n = m_vertices.size(); i < n; ++i) {
		m_vertices[i] += offset;
	}
	m_rect.translate(offset);
}

void ChainShape::Draw(const d2d::Matrix& mt, const d2d::ColorTrans& color) const
{
	if (m_vertices.empty()) return;

	d2d::PrimitiveDraw::drawPolyline(mt, m_vertices, color.multi, m_isLoop);
	if (d2d::Settings::ctlPosSize != 0) {
		d2d::PrimitiveDraw::drawCircles(m_vertices, d2d::Settings::ctlPosSize, true, 2, d2d::Colorf(0.4f, 0.8f, 0.4f));
	}
}

d2d::IPropertySetting* ChainShape::CreatePropertySetting(d2d::EditPanelImpl* stage)
{
	return new ChainPropertySetting(stage, this);
}

void ChainShape::LoadFromFile(const Json::Value& value, const std::string& dir)
{
	d2d::IShape::LoadFromFile(value, dir);

	size_t num = value["vertices"]["x"].size();
	m_vertices.resize(num);
	for (size_t i = 0; i < num; ++i) {
		m_vertices[i].x = value["vertices"]["x"][i].asDouble();
		m_vertices[i].y = value["vertices"]["y"][i].asDouble();
	}

	m_isLoop = value["closed"].asBool();

	InitBounding();
}

void ChainShape::StoreToFile(Json::Value& value, const std::string& dir) const
{
	d2d::IShape::StoreToFile(value, dir);

	for (size_t i = 0, n = m_vertices.size(); i < n; ++i) {
		value["vertices"]["x"][i] = m_vertices[i].x;
		value["vertices"]["y"][i] = m_vertices[i].y;
	}
	value["closed"] = IsClosed();
}

void ChainShape::Add(size_t index, const d2d::Vector& pos)
{
	if (index <= m_vertices.size())
		m_vertices.insert(m_vertices.begin() + index, pos);
}

void ChainShape::Remove(const d2d::Vector& pos)
{
	std::vector<d2d::Vector>::iterator itr = m_vertices.begin();
	for ( ; itr != m_vertices.end(); ++itr) {
		if (*itr == pos) {
			m_vertices.erase(itr);
			break;
		}
	}
}

void ChainShape::Change(const d2d::Vector& from, const d2d::Vector& to)
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
		InitBounding();
	} else {
		m_rect.combine(to);
	}
}

void ChainShape::Load(const std::vector<d2d::Vector>& vertices)
{
	m_vertices = vertices;
	InitBounding();
}

void ChainShape::InitBounding()
{
	m_rect.makeInfinite();
	for (size_t i = 0, n = m_vertices.size(); i < n; ++i)
		m_rect.combine(m_vertices[i]);
}

}