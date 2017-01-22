#include "NetworkShape.h"
#include "color_config.h"

#include <ee/Math2D.h>
#include <ee/SettingData.h>

#include <easyshape.h>

#include <SM_Calc.h>
#include <sprite2/S2_RVG.h>
#include <gum/JsonSerializer.h>

namespace emesh
{

static const int NODE_RADIUS = 5;

NetworkShape::NetworkShape(const NetworkShape& nw)
	: s2::Shape(nw)
	, eshape::EditedPolyShape(nw)
	, s2::NetworkShape(nw)
	, m_node_radius(nw.m_node_radius)
{
}

NetworkShape::NetworkShape(const std::vector<sm::vec2>& vertices,
						   float node_radius)
	: s2::Shape()
	, eshape::EditedPolyShape()
	, s2::NetworkShape(vertices)
	, m_node_radius(node_radius)
{
}

void NetworkShape::Draw(const sm::mat4& mt, const s2::RenderColor* color) const
{
	s2::NetworkShape::Draw(mt, color);

	s2::RVG::SetColor(BLUE);
	s2::RVG::Circles(m_inner_vertices, m_node_radius, true);
}

void NetworkShape::LoadFromFile(const Json::Value& value, const std::string& dir)
{
	const Json::Value& shape_val = value["shape"];
	std::vector<sm::vec2> vertices;
	gum::JsonSerializer::Load(shape_val["outline"], vertices);
	m_vertices = vertices;
	UpdateBounding();
	gum::JsonSerializer::Load(shape_val["inner"], m_inner_vertices);
}

void NetworkShape::StoreToFile(Json::Value& value, const std::string& dir) const
{
	Json::Value& shape_val = value["shape"];
	gum::JsonSerializer::Store(m_vertices, shape_val["outline"]);
	gum::JsonSerializer::Store(m_inner_vertices, shape_val["inner"]);
}

bool NetworkShape::InsertInner(const sm::vec2& pos)
{
	for (int i = 0, n = m_vertices.size(); i < n; ++i) {
		if (m_vertices[i] == pos) {
			return false;
		}
	}
	if (IsIntersect(sm::rect(pos, NODE_RADIUS, NODE_RADIUS)) || !IsContain(pos)) {
		return false;
	}
	m_inner_vertices.push_back(pos);
	return true;
}

bool NetworkShape::RemoveInner(const sm::vec2& pos)
{
	for (int i = 0, n = m_inner_vertices.size(); i < n; ++i) {
		const sm::vec2& p = m_inner_vertices[i];
		if (sm::dis_pos_to_pos(p, pos) < NODE_RADIUS) {
			m_inner_vertices.erase(m_inner_vertices.begin() + i);
			return true;
		}
	}
	return false;
}

sm::vec2* NetworkShape::QueryInner(const sm::vec2& pos)
{
	for (int i = 0, n = m_inner_vertices.size(); i < n; ++i) {
		const sm::vec2& p = m_inner_vertices[i];
		if (sm::dis_pos_to_pos(p, pos) < NODE_RADIUS) {
			return &m_inner_vertices[i];
		}
	}
	return NULL;
}

void NetworkShape::AddVertex(int index, const sm::vec2& pos)
{
	eshape::PolylineEditor::AddVertex(m_vertices, m_bounding, index, pos);
}

void NetworkShape::RemoveVertex(const sm::vec2& pos)
{
	eshape::PolylineEditor::RemoveVertex(m_vertices, m_bounding, pos);
}

void NetworkShape::ChangeVertex(const sm::vec2& from, const sm::vec2& to)
{
	eshape::PolylineEditor::ChangeVertex(m_vertices, m_bounding, from, to);
}

void NetworkShape::SetVertices(const std::vector<sm::vec2>& vertices)
{
	m_vertices = vertices;
	UpdateBounding();
}

}