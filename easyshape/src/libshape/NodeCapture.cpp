#include "NodeCapture.h"
#include "ShapeType.h"

#include "PointShape.h"
#include "BezierShape.h"
#include "ChainShape.h"
#include "CircleShape.h"
#include "CosineShape.h"
#include "PolygonShape.h"
#include "RectShape.h"

namespace libshape
{

NodeCapture::NodeCapture(d2d::MultiShapesImpl* shapesImpl, int tol)
	: m_shapesImpl(shapesImpl)
	, m_tol(tol)
{
}

void NodeCapture::captureEditable(const d2d::Vector& pos, NodeAddr& result)
{
	m_shapesImpl->TraverseShapes(RectQueryVisitor(pos, m_tol, result), d2d::DT_EDITABLE);
}

void NodeCapture::captureSelectable(const d2d::Vector& pos, NodeAddr& result)
{
	m_shapesImpl->TraverseShapes(RectQueryVisitor(pos, m_tol, result), d2d::DT_SELECTABLE);
}

//////////////////////////////////////////////////////////////////////////
// class NodeCapture::RectQueryVisitor
//////////////////////////////////////////////////////////////////////////

NodeCapture::RectQueryVisitor::
RectQueryVisitor(const d2d::Vector& pos, float tolerance, NodeAddr& result)
	: m_pos(pos)
	, m_tolerance(tolerance)
	, m_rect(pos, tolerance, tolerance)
	, m_result(result)
{
	result.shape = NULL;
}

void NodeCapture::RectQueryVisitor::
Visit(d2d::Object* object, bool& next)
{
	next = true;

	d2d::Shape* shape = dynamic_cast<d2d::Shape*>(object);
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
		next = !Visit(static_cast<ChainShape*>(shape));
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
	if (d2d::Math2D::GetDistance(point->GetPos(), m_pos) < m_tolerance) {
		m_result.shape = point;
		m_result.pos.SetInvalid();
		return true;
	} else {
		return false;
	}
}

bool NodeCapture::RectQueryVisitor::
Visit(BezierShape* bezier)
{
	// capture center
	const d2d::Rect& rect = bezier->GetRect();
	if (d2d::Math2D::GetDistance(d2d::Vector(rect.CenterX(), rect.CenterY()), m_pos) < m_tolerance)
	{
		m_result.shape = bezier;
		m_result.pos.SetInvalid();
		return true;
	}

	// capture control points
	const d2d::Vector* ctrl_nodes = bezier->GetCtrlNode();
	for (int i = 0; i < BezierShape::CTRL_NODE_COUNT; ++i) {
		if (d2d::Math2D::GetDistance(ctrl_nodes[i], m_pos) < m_tolerance) {
			m_result.shape = bezier;
			m_result.pos = ctrl_nodes[i];
			return true;
		}
	}

	return false;
}

bool NodeCapture::RectQueryVisitor::
Visit(ChainShape* chain)
{
	// capture center
	const d2d::Rect& rect = chain->GetRect();
	if (d2d::Math2D::GetDistance(d2d::Vector(rect.CenterX(), rect.CenterY()), m_pos) < m_tolerance)
	{
		m_result.shape = chain;
		m_result.pos.SetInvalid();
		return true;
	}

	// capture control points
	if (!d2d::Math2D::IsRectIntersectRect(m_rect, chain->GetRect()))
		return false;

	if (!chain->IsIntersect(m_rect)) 
		return false;

	const std::vector<d2d::Vector>& vertices = chain->GetVertices();
	for (size_t i = 0, n = vertices.size(); i < n; ++i)
	{
		if (d2d::Math2D::GetDistance(vertices[i], m_pos) < m_tolerance)
		{
			m_result.shape = chain;
			m_result.pos = vertices[i];
			return true;
		}
	}

	return false;
}

bool NodeCapture::RectQueryVisitor::
Visit(CircleShape* circle)
{
	const float dis = d2d::Math2D::GetDistance(circle->center, m_pos);

	// capture center
	if (dis < m_tolerance)
	{
		m_result.shape = circle;
		m_result.pos = circle->center;
		return true;
	}
	// capture ring
	else if (dis < circle->radius + m_tolerance 
		&& dis > circle->radius - m_tolerance)
	{
		m_result.shape = circle;
		m_result.pos.SetInvalid();
		return true;
	}

	return false;
}

bool NodeCapture::RectQueryVisitor::
Visit(RectShape* rect)
{
	// capture center
	if (d2d::Math2D::GetDistance(m_pos, d2d::Vector(rect->m_rect.CenterX(), rect->m_rect.CenterY())) < m_tolerance)
	{
		m_result.shape = rect;
		m_result.pos.SetInvalid();
		return true;
	}
	// capture edge
	else if (d2d::Math2D::GetDistance(m_pos, d2d::Vector(rect->m_rect.xmin, rect->m_rect.ymin)) < m_tolerance)
	{
		m_result.shape = rect;
		m_result.pos = d2d::Vector(rect->m_rect.xmin, rect->m_rect.ymin);
		return true;
	}
	else if (d2d::Math2D::GetDistance(m_pos, d2d::Vector(rect->m_rect.xmin, rect->m_rect.ymax)) < m_tolerance)
	{
		m_result.shape = rect;
		m_result.pos = d2d::Vector(rect->m_rect.xmin, rect->m_rect.ymax);
		return true;
	}
	else if (d2d::Math2D::GetDistance(m_pos, d2d::Vector(rect->m_rect.xmax, rect->m_rect.ymax)) < m_tolerance)
	{
		m_result.shape = rect;
		m_result.pos = d2d::Vector(rect->m_rect.xmax, rect->m_rect.ymax);
		return true;
	}
	else if (d2d::Math2D::GetDistance(m_pos, d2d::Vector(rect->m_rect.xmax, rect->m_rect.ymin)) < m_tolerance)
	{
		m_result.shape = rect;
		m_result.pos = d2d::Vector(rect->m_rect.xmax, rect->m_rect.ymin);
		return true;
	}
	return false;
}

}