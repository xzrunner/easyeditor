#include "BezierShape.h"
#include "BezierPropertySetting.h"

#include <ee/Math2D.h>

namespace eshape
{

BezierShape::BezierShape(const BezierShape& bezier)
	: ee::Shape(bezier)
	, s2::BezierShape(bezier)
{
}

BezierShape::BezierShape(const sm::vec2& start, const sm::vec2& end)
	: s2::BezierShape(start, end)
{
}

void BezierShape::Translate(const sm::vec2& offset)
{
	for (int i = 0; i < CTRL_NODE_COUNT; ++i) {
		m_control_nodes[i] += offset;
	}
	for (int i = 0, n = m_vertices.size(); i < n; ++i) {
		m_vertices[i] += offset;
	}
	m_bounding.Translate(offset);
}

ee::PropertySetting* BezierShape::CreatePropertySetting(ee::EditPanelImpl* stage)
{
	return new BezierPropertySetting(stage, this);
}

void BezierShape::LoadFromFile(const Json::Value& value, const std::string& dir)
{
	ee::Shape::LoadFromFile(value, dir);

	for (int i = 0; i < 4; ++i)
	{
		m_control_nodes[i].x = value["points"]["x"][i].asDouble();
		m_control_nodes[i].y = value["points"]["y"][i].asDouble();
	}

	UpdatePolyline();
}

void BezierShape::StoreToFile(Json::Value& value, const std::string& dir) const
{
	ee::Shape::StoreToFile(value, dir);

	for (int i = 0; i < BezierShape::CTRL_NODE_COUNT; ++i) {
		value["points"]["x"][i] = m_control_nodes[i].x;
		value["points"]["y"][i] = m_control_nodes[i].y;
	}
}

void BezierShape::Mirror(bool x, bool y)
{
	sm::vec2 center = GetBounding().Center();
	for (int i = 0; i < CTRL_NODE_COUNT; ++i) 
	{
		if (x) {
			m_control_nodes[i].x = center.x * 2 - m_control_nodes[i].x;
		}
		if (y) {
			m_control_nodes[i].y = center.y * 2 - m_control_nodes[i].y;			
		}
	}
	UpdatePolyline();
}

void BezierShape::MoveCtrlNode(const sm::vec2& from, const sm::vec2& to)
{
	for (int i = 0; i < CTRL_NODE_COUNT; ++i) {
		if (m_control_nodes[i] == from) {
			m_control_nodes[i] = to;
			UpdatePolyline();
			return;
		}
	}
}

}