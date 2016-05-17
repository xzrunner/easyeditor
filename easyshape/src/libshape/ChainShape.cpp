#include "ChainShape.h"
#include "ChainPropertySetting.h"

#include <ee/Math2D.h>
#include <ee/EE_RVG.h>
#include <ee/SettingData.h>

namespace eshape
{

ChainShape::ChainShape()
{
	m_loop = false;
	m_draw_dir = true;
}

ChainShape::ChainShape(const ChainShape& chain)
{
	copy(chain.m_vertices.begin(), chain.m_vertices.end(), back_inserter(m_vertices));
	m_loop = chain.m_loop;
	m_draw_dir = chain.m_draw_dir;
	m_rect = chain.m_rect;
}

ChainShape::ChainShape(const std::vector<sm::vec2>& vertices, bool loop)
	: m_vertices(vertices)
{
	m_loop = loop;
	m_draw_dir = !loop;
	InitBounding();
}

ChainShape* ChainShape::Clone() const
{
	return new ChainShape(*this);
}

bool ChainShape::IsContain(const sm::vec2& pos) const
{
	if (m_loop) 
	{
		return ee::Math2D::IsPointInArea(pos, m_vertices);
	}
	else
	{
		sm::rect rect(m_rect);
		rect.xmin -= QUERY_ACCURACY;
		rect.xmax += QUERY_ACCURACY;
		rect.ymin -= QUERY_ACCURACY;
		rect.ymax += QUERY_ACCURACY;
		if (m_vertices.empty() || !ee::Math2D::IsPointInRect(pos, rect))
			return false;

		size_t index;
		float dis = ee::Math2D::GetDisPointToMultiPos(pos, m_vertices, &index);
		if (dis < QUERY_ACCURACY) return true;

		if (m_vertices.size() < 2) return false;
		for (size_t i = 0, n = m_vertices.size() - 1; i < n; ++i)
		{
			float dis = ee::Math2D::GetDisPointToSegment(pos, m_vertices[i], m_vertices[i + 1]);
			if (dis < QUERY_ACCURACY) 
				return true;
		}

		return false;
	}
}

bool ChainShape::IsIntersect(const sm::rect& rect) const
{
	if (m_vertices.empty() || !ee::Math2D::IsRectIntersectRect(rect, m_rect))	
		return false;

	for (size_t i = 0, n = m_vertices.size(); i < n; ++i)
	{
		if (ee::Math2D::IsPointInRect(m_vertices[i], rect))
			return true;
	}

	if (m_vertices.size() < 2) return false;
	for (size_t i = 0, n = m_vertices.size() - 1; i < n; ++i)
	{
		if (ee::Math2D::IsSegmentIntersectRect(m_vertices[i], m_vertices[i + 1], rect))
			return true;
	}

	if (m_loop && ee::Math2D::IsSegmentIntersectRect(m_vertices.front(), m_vertices.back(), rect))
		return true;

	return false;
}

void ChainShape::Translate(const sm::vec2& offset)
{
	for (int i = 0, n = m_vertices.size(); i < n; ++i) {
		m_vertices[i] += offset;
	}
	m_rect.Translate(offset);
}

void ChainShape::Draw(const sm::mat4& mt, const s2::RenderColor& color) const
{
	if (m_vertices.empty()) return;

	std::vector<sm::vec2> vertices;
	ee::Math2D::TransVertices(mt, m_vertices, vertices);
	ee::RVG::Color(color.mul);
	ee::RVG::Polyline(vertices, m_loop);
	if (ee::SettingData::ctl_pos_sz != 0) {
		float len = ee::Math2D::TransLen(ee::SettingData::ctl_pos_sz, mt);
		ee::RVG::Color(s2::Color(102, 204, 102));
		ee::RVG::Circles(vertices, len, true);
	}
	if (m_draw_dir) {
		ee::RVG::Color(color.mul);
		ee::RVG::Circle(vertices[0], 10, true);
	}
}

ee::PropertySetting* ChainShape::CreatePropertySetting(ee::EditPanelImpl* stage)
{
	return new ChainPropertySetting(stage, this);
}

void ChainShape::LoadFromFile(const Json::Value& value, const std::string& dir)
{
	ee::Shape::LoadFromFile(value, dir);

	size_t num = value["vertices"]["x"].size();
	m_vertices.resize(num);
	for (size_t i = 0; i < num; ++i) {
		m_vertices[i].x = value["vertices"]["x"][i].asDouble();
		m_vertices[i].y = value["vertices"]["y"][i].asDouble();
	}

	m_loop = value["closed"].asBool();
	m_draw_dir = !m_loop;

	InitBounding();
}

void ChainShape::StoreToFile(Json::Value& value, const std::string& dir) const
{
	ee::Shape::StoreToFile(value, dir);

	for (size_t i = 0, n = m_vertices.size(); i < n; ++i) {
		value["vertices"]["x"][i] = m_vertices[i].x;
		value["vertices"]["y"][i] = m_vertices[i].y;
	}
	value["closed"] = IsClosed();
}

void ChainShape::Add(size_t index, const sm::vec2& pos)
{
	if (index <= m_vertices.size())
		m_vertices.insert(m_vertices.begin() + index, pos);
}

void ChainShape::Remove(const sm::vec2& pos)
{
	std::vector<sm::vec2>::iterator itr = m_vertices.begin();
	for ( ; itr != m_vertices.end(); ++itr) {
		if (*itr == pos) {
			m_vertices.erase(itr);
			break;
		}
	}
}

void ChainShape::Change(const sm::vec2& from, const sm::vec2& to)
{
	size_t index = 0;
	for (size_t n = m_vertices.size(); index < n; ++index)
	{
		if (m_vertices[index] == from)
			break;
	}

	if (index == m_vertices.size()) return;

	m_vertices[index] = to;
	if (from.x == m_rect.xmin || from.x == m_rect.xmax || 
		from.y == m_rect.ymin || from.y == m_rect.ymax) {
		InitBounding();
	} else {
		m_rect.Combine(to);
	}
}

void ChainShape::Load(const std::vector<sm::vec2>& vertices)
{
	m_vertices = vertices;
	InitBounding();
}

void ChainShape::InitBounding()
{
	m_rect.MakeEmpty();
	for (size_t i = 0, n = m_vertices.size(); i < n; ++i)
		m_rect.Combine(m_vertices[i]);
}

}