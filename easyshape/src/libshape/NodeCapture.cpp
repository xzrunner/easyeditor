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
	m_shapesImpl->traverseShapes(RectQueryVisitor(pos, m_tol, result), d2d::DT_EDITABLE);
}

void NodeCapture::captureSelectable(const d2d::Vector& pos, NodeAddr& result)
{
	m_shapesImpl->traverseShapes(RectQueryVisitor(pos, m_tol, result), d2d::DT_SELECTABLE);
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
visit(d2d::Object* object, bool& bFetchNext)
{
	bFetchNext = true;

	d2d::IShape* shape = dynamic_cast<d2d::IShape*>(object);
	if (!shape) {
		return;
	}

	ShapeType type = get_shape_type(shape->GetShapeDesc());
	switch (type)
	{
	case ST_POINT:
		bFetchNext = !visit(static_cast<PointShape*>(shape));
		break;
	case ST_BEZIER:
		bFetchNext = !visit(static_cast<BezierShape*>(shape));
		break;
	case ST_CHAIN: case ST_POLYGON: case ST_COMPLEX_POLYGON: case ST_COSINE_CURVE:
		bFetchNext = !visit(static_cast<ChainShape*>(shape));
		break;
	case ST_CIRCLE:
		bFetchNext = !visit(static_cast<CircleShape*>(shape));
		break;
	case ST_RECT:
		bFetchNext = !visit(static_cast<RectShape*>(shape));
		break;
	}
}

bool NodeCapture::RectQueryVisitor::
visit(PointShape* point)
{
	if (d2d::Math::getDistance(point->GetPos(), m_pos) < m_tolerance) {
		m_result.shape = point;
		m_result.pos.setInvalid();
		return true;
	} else {
		return false;
	}
}

bool NodeCapture::RectQueryVisitor::
visit(BezierShape* bezier)
{
	// capture center
	const d2d::Rect& rect = bezier->getRect();
	if (d2d::Math::getDistance(d2d::Vector(rect.xCenter(), rect.yCenter()), m_pos) < m_tolerance)
	{
		m_result.shape = bezier;
		m_result.pos.setInvalid();
		return true;
	}

	// capture control points
	const d2d::Vector* ctrl_nodes = bezier->GetCtrlNode();
	for (int i = 0; i < BezierShape::CTRL_NODE_COUNT; ++i) {
		if (d2d::Math::getDistance(ctrl_nodes[i], m_pos) < m_tolerance) {
			m_result.shape = bezier;
			m_result.pos = ctrl_nodes[i];
			return true;
		}
	}

	return false;
}

bool NodeCapture::RectQueryVisitor::
visit(ChainShape* chain)
{
	// capture center
	const d2d::Rect& rect = chain->getRect();
	if (d2d::Math::getDistance(d2d::Vector(rect.xCenter(), rect.yCenter()), m_pos) < m_tolerance)
	{
		m_result.shape = chain;
		m_result.pos.setInvalid();
		return true;
	}

	// capture control points
	if (!d2d::Math::isRectIntersectRect(m_rect, chain->getRect()))
		return false;

	if (!chain->isIntersect(m_rect)) 
		return false;

	const std::vector<d2d::Vector>& vertices = chain->GetVertices();
	for (size_t i = 0, n = vertices.size(); i < n; ++i)
	{
		if (d2d::Math::getDistance(vertices[i], m_pos) < m_tolerance)
		{
			m_result.shape = chain;
			m_result.pos = vertices[i];
			return true;
		}
	}

	return false;
}

bool NodeCapture::RectQueryVisitor::
visit(CircleShape* circle)
{
	const float dis = d2d::Math::getDistance(circle->center, m_pos);

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
		m_result.pos.setInvalid();
		return true;
	}

	return false;
}

bool NodeCapture::RectQueryVisitor::
visit(RectShape* rect)
{
	// capture center
	if (d2d::Math::getDistance(m_pos, d2d::Vector(rect->m_rect.xCenter(), rect->m_rect.yCenter())) < m_tolerance)
	{
		m_result.shape = rect;
		m_result.pos.setInvalid();
		return true;
	}
	// capture edge
	else if (d2d::Math::getDistance(m_pos, d2d::Vector(rect->m_rect.xMin, rect->m_rect.yMin)) < m_tolerance)
	{
		m_result.shape = rect;
		m_result.pos = d2d::Vector(rect->m_rect.xMin, rect->m_rect.yMin);
		return true;
	}
	else if (d2d::Math::getDistance(m_pos, d2d::Vector(rect->m_rect.xMin, rect->m_rect.yMax)) < m_tolerance)
	{
		m_result.shape = rect;
		m_result.pos = d2d::Vector(rect->m_rect.xMin, rect->m_rect.yMax);
		return true;
	}
	else if (d2d::Math::getDistance(m_pos, d2d::Vector(rect->m_rect.xMax, rect->m_rect.yMax)) < m_tolerance)
	{
		m_result.shape = rect;
		m_result.pos = d2d::Vector(rect->m_rect.xMax, rect->m_rect.yMax);
		return true;
	}
	else if (d2d::Math::getDistance(m_pos, d2d::Vector(rect->m_rect.xMax, rect->m_rect.yMin)) < m_tolerance)
	{
		m_result.shape = rect;
		m_result.pos = d2d::Vector(rect->m_rect.xMax, rect->m_rect.yMin);
		return true;
	}
	return false;
}

}