#include "NetworkShape.h"
#include "color_config.h"

#include <ee/Math2D.h>
#include <ee/SettingData.h>
#include <ee/EE_RVG.h>
#include <ee/JsonSerializer.h>

namespace emesh
{

static const int NODE_RADIUS = 5;

NetworkShape::NetworkShape(const NetworkShape& nw)
	: eshape::ChainShape(nw)
	, m_node_radius(nw.m_node_radius)
{
	m_inner_vertices = nw.m_inner_vertices;
}

NetworkShape::NetworkShape(eshape::ChainShape* loop,
						   float node_radius)
	: eshape::ChainShape(*loop)
	, m_node_radius(node_radius)
{
}

NetworkShape* NetworkShape::Clone() const
{
	return new NetworkShape(*this);
}

void NetworkShape::Draw(const sm::mat4& mt, const ee::RenderColor& color) const
{
	eshape::ChainShape::Draw(mt, color);

	ee::RVG::Color(BLUE);
	ee::RVG::Circles(m_inner_vertices, m_node_radius, true);
}

void NetworkShape::LoadFromFile(const Json::Value& value, const std::string& dir)
{
	const Json::Value& shape_val = value["shape"];
	ee::JsonSerializer::Load(shape_val["outline"], m_vertices);
	ee::JsonSerializer::Load(shape_val["inner"], m_inner_vertices);
}

void NetworkShape::StoreToFile(Json::Value& value, const std::string& dir) const
{
	Json::Value& shape_val = value["shape"];
	ee::JsonSerializer::Store(m_vertices, shape_val["outline"]);
	ee::JsonSerializer::Store(m_inner_vertices, shape_val["inner"]);
}

bool NetworkShape::InsertInner(const sm::vec2& pos)
{
	if (IsIntersect(ee::Rect(pos, NODE_RADIUS, NODE_RADIUS)) || !IsContain(pos)) {
		return false;
	}
	m_inner_vertices.push_back(pos);
	return true;
}

bool NetworkShape::RemoveInner(const sm::vec2& pos)
{
	for (int i = 0, n = m_inner_vertices.size(); i < n; ++i) {
		const sm::vec2& p = m_inner_vertices[i];
		if (ee::Math2D::GetDistance(p, pos) < NODE_RADIUS) {
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
		if (ee::Math2D::GetDistance(p, pos) < NODE_RADIUS) {
			return &m_inner_vertices[i];
		}
	}
	return NULL;
}

}