#include "ChainShape.h"
#include "ChainPropertySetting.h"
#include "PolylineEditor.h"

#include <ee/Math2D.h>
#include <ee/SettingData.h>

#include <SM_Calc.h>
#include <sprite2/S2_RVG.h>

namespace eshape
{

ChainShape::ChainShape()
{
	m_draw_dir = true;
}

ChainShape::ChainShape(const ChainShape& chain) 
	: eshape::EditedPolyShape(chain)
	, s2::PolylineShape(chain)
	, m_draw_dir(chain.m_draw_dir)
{
}

ChainShape::ChainShape(const std::vector<sm::vec2>& vertices, bool closed)
	: s2::PolylineShape(vertices, closed)
	, m_draw_dir(!closed)
{
}

void ChainShape::Draw(const sm::mat4& mt, const s2::RenderColor* color) const
{
	s2::PolylineShape::Draw(mt, color);

	if (m_draw_dir && m_vertices.size() >= 2) {
		s2::RVG::Arrow(m_vertices[m_vertices.size() - 2], m_vertices[m_vertices.size() - 1]);
	}
}

void ChainShape::Translate(const sm::vec2& offset)
{
	for (int i = 0, n = m_vertices.size(); i < n; ++i) {
		m_vertices[i] += offset;
	}
	m_bounding.Translate(offset);
}

ee::PropertySetting* ChainShape::CreatePropertySetting(ee::EditPanelImpl* stage)
{
	return new ChainPropertySetting(stage, this);
}

void ChainShape::LoadFromFile(const Json::Value& value, const std::string& dir)
{
	ee::Shape::LoadFromFile(value, dir);

	int num = value["vertices"]["x"].size();
	m_vertices.resize(num);
	for (int i = 0; i < num; ++i) {
		m_vertices[i].x = value["vertices"]["x"][i].asDouble();
		m_vertices[i].y = value["vertices"]["y"][i].asDouble();
	}

	m_closed = value["closed"].asBool();
	m_draw_dir = !m_closed;

	UpdateBounding();
}

void ChainShape::StoreToFile(Json::Value& value, const std::string& dir) const
{
	ee::Shape::StoreToFile(value, dir);

	for (size_t i = 0, n = m_vertices.size(); i < n; ++i) {
		value["vertices"]["x"][i] = m_vertices[i].x;
		value["vertices"]["y"][i] = m_vertices[i].y;
	}
	value["closed"] = m_closed;
}

void ChainShape::AddVertex(int index, const sm::vec2& pos)
{
	PolylineEditor::AddVertex(m_vertices, m_bounding, index, pos);
}

void ChainShape::RemoveVertex(const sm::vec2& pos)
{
	PolylineEditor::RemoveVertex(m_vertices, m_bounding, pos);
}

void ChainShape::ChangeVertex(const sm::vec2& from, const sm::vec2& to)
{
	PolylineEditor::ChangeVertex(m_vertices, m_bounding, from, to);
}

void ChainShape::SetVertices(const std::vector<sm::vec2>& vertices)
{
	m_vertices = vertices;
	UpdateBounding();
}

}