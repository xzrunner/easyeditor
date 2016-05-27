#include "NodeCapture.h"
#include "ShapeType.h"

#include "PointShape.h"
#include "CircleShape.h"
#include "BezierShape.h"
#include "PolylineShape.h"
#include "CosineShape.h"
#include "PolygonShape.h"
#include "RectShape.h"

#include <ee/MultiShapesImpl.h>
#include <ee/Math2D.h>

namespace eshape
{

NodeCapture::NodeCapture(ee::MultiShapesImpl* shapes_impl, int tol)
	: m_shapes_impl(shapes_impl)
	, m_tol(tol)
{
}

void NodeCapture::captureEditable(const sm::vec2& pos, NodeAddr& result)
{
	m_shapes_impl->TraverseShapes(RectQueryVisitor(pos, m_tol, result), ee::DT_EDITABLE);
}

void NodeCapture::captureSelectable(const sm::vec2& pos, NodeAddr& result)
{
	m_shapes_impl->TraverseShapes(RectQueryVisitor(pos, m_tol, result), ee::DT_SELECTABLE);
}

//////////////////////////////////////////////////////////////////////////
// class NodeCapture::RectQueryVisitor
//////////////////////////////////////////////////////////////////////////

NodeCapture::RectQueryVisitor::
RectQueryVisitor(const sm::vec2& pos, float tolerance, NodeAddr& result)
	: m_pos(pos)
	, m_tolerance(tolerance)
	, m_rect(pos, tolerance, tolerance)
	, m_result(result)
{
	result.shape = NULL;
}

void NodeCapture::RectQueryVisitor::
Visit(ee::Object* object, bool& next)
{
	next = true;

	ee::Shape* shape = dynamic_cast<ee::Shape*>(object);
	if (!shape) {
		return;
	}

	ShapeType type = get_shape_type(shape->GetShapeDesc());
	switch (type)
	{
	case ST_POINT:
		next = !Visit(static_cast<PointShape*>(shape));
		break;
	case ST_BEZIER:
		next = !Visit(static_cast<BezierShape*>(shape));
		break;
	case ST_CHAIN: case ST_POLYGON: case ST_COMPLEX_POLYGON: case ST_COSINE_CURVE:
		next = !Visit(static_cast<PolylineShape*>(shape));
		break;
	case ST_CIRCLE:
		next = !Visit(static_cast<CircleShape*>(shape));
		break;
	case ST_RECT:
		next = !Visit(static_cast<RectShape*>(shape));
		break;
	}
}

bool NodeCapture::RectQueryVisitor::
Visit(PointShape* point)
{
	if (ee::Math2D::GetDistance(point->GetPos(), m_pos) < m_tolerance) {
		m_result.shape = point;
		m_result.pos_valid = false;
		return true;
	} else {
		return false;
	}
}

bool NodeCapture::RectQueryVisitor::
Visit(BezierShape* bezier)
{
	// capture center
	const sm::rect& rect = bezier->GetRect();
	if (ee::Math2D::GetDistance(rect.Center(), m_pos) < m_tolerance)
	{
		m_result.shape = bezier;
		m_result.pos_valid = false;
		return true;
	}

	// capture control points
	const sm::vec2* ctrl_nodes = bezier->GetCtrlNodes();
	for (int i = 0; i < s2::BezierShape::CTRL_NODE_COUNT; ++i) {
		if (ee::Math2D::GetDistance(ctrl_nodes[i], m_pos) < m_tolerance) {
			m_result.shape = bezier;
			m_result.pos = ctrl_nodes[i];
			m_result.pos_valid = true;
			return true;
		}
	}

	return false;
}

bool NodeCapture::RectQueryVisitor::
Visit(PolylineShape* polyline)
{
	// capture center
	const sm::rect& rect = polyline->GetRect();
	if (ee::Math2D::GetDistance(rect.Center(), m_pos) < m_tolerance)
	{
		m_result.shape = polyline;
		m_result.pos_valid = false;
		return true;
	}

	// capture control points
	if (!ee::Math2D::IsRectIntersectRect(m_rect, polyline->GetRect()))
		return false;

	if (!polyline->IsIntersect(m_rect)) 
		return false;

	const std::vector<sm::vec2>& vertices = polyline->GetVertices();
	for (size_t i = 0, n = vertices.size(); i < n; ++i)
	{
		if (ee::Math2D::GetDistance(vertices[i], m_pos) < m_tolerance)
		{
			m_result.shape = polyline;
			m_result.pos = vertices[i];
			m_result.pos_valid = true;
			return true;
		}
	}

	return false;
}

bool NodeCapture::RectQueryVisitor::
Visit(CircleShape* circle)
{
	const float dis = ee::Math2D::GetDistance(circle->GetCenter(), m_pos);

	// capture center
	if (dis < m_tolerance)
	{
		m_result.shape = circle;
		m_result.pos = circle->GetCenter();
		m_result.pos_valid = true;
		return true;
	}
	// capture ring
	else if (dis < circle->GetRadius() + m_tolerance 
		&& dis > circle->GetRadius() - m_tolerance)
	{
		m_result.shape = circle;
		m_result.pos_valid = false;
		return true;
	}

	return false;
}

bool NodeCapture::RectQueryVisitor::
Visit(RectShape* rect)
{
	// capture center
	if (ee::Math2D::GetDistance(m_pos, rect->GetRect().Center()) < m_tolerance)
	{
		m_result.shape = rect;
		m_result.pos_valid = false;
		return true;
	}
	// capture edge
	else if (ee::Math2D::GetDistance(m_pos, sm::vec2(rect->GetRect().xmin, rect->GetRect().ymin)) < m_tolerance)
	{
		m_result.shape = rect;
		m_result.pos = sm::vec2(rect->GetRect().xmin, rect->GetRect().ymin);
		m_result.pos_valid = true;
		return true;
	}
	else if (ee::Math2D::GetDistance(m_pos, sm::vec2(rect->GetRect().xmin, rect->GetRect().ymax)) < m_tolerance)
	{
		m_result.shape = rect;
		m_result.pos = sm::vec2(rect->GetRect().xmin, rect->GetRect().ymax);
		m_result.pos_valid = true;
		return true;
	}
	else if (ee::Math2D::GetDistance(m_pos, sm::vec2(rect->GetRect().xmax, rect->GetRect().ymax)) < m_tolerance)
	{
		m_result.shape = rect;
		m_result.pos = sm::vec2(rect->GetRect().xmax, rect->GetRect().ymax);
		m_result.pos_valid = true;
		return true;
	}
	else if (ee::Math2D::GetDistance(m_pos, sm::vec2(rect->GetRect().xmax, rect->GetRect().ymin)) < m_tolerance)
	{
		m_result.shape = rect;
		m_result.pos = sm::vec2(rect->GetRect().xmax, rect->GetRect().ymin);
		m_result.pos_valid = true;
		return true;
	}
	return false;
}

}