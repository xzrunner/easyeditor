#include "EditMeshOP.h"
#include "StagePanel.h"
#include "Triangle.h"
#include "Shape.h"

namespace emesh
{

static const int CENTER_RADIUS = 3;
static const int CENTER_EDGE = 10;

EditMeshOP::EditMeshOP(StagePanel* stage)
	: SelectNodesOP(stage)
	, m_stage(stage)
	, m_bRightPress(false)
	, m_selCenter(false)
{
	m_lastPos.setInvalid();
	m_center.set(0, 0);
}

bool EditMeshOP::onMouseLeftDown(int x, int y)
{
	m_lastPos = m_stage->transPosScreenToProject(x, y);
	if (d2d::Math::getDistance(m_lastPos, m_center) < CENTER_RADIUS)
	{
		m_selCenter = true;
		return false;
	}

	if (SelectNodesOP::onMouseLeftDown(x, y))
		return true;

	m_bRightPress = false;

	return false;
}

bool EditMeshOP::onMouseLeftUp(int x, int y)
{
	if (SelectNodesOP::onMouseLeftUp(x, y))
		return true;

	m_selCenter = false;

	return false;
}

bool EditMeshOP::onMouseRightDown(int x, int y)
{
	if (SelectNodesOP::onMouseRightDown(x, y))
		return true;

	m_lastPos = m_stage->transPosScreenToProject(x, y);

	m_bRightPress = true;

	return false;
}

bool EditMeshOP::onMouseDrag(int x, int y)
{
	if (m_selCenter)
	{
		d2d::Vector pos = m_stage->transPosScreenToProject(x, y);
		m_center = pos;
		m_stage->Refresh();
		return true;
	}

	if (SelectNodesOP::onMouseDrag(x, y))
		return true;

	Shape* shape = m_stage->GetShape();
	if (!shape) return false;

	if (!m_selection.IsEmpty())
	{
		d2d::Vector pos = m_stage->transPosScreenToProject(x, y);
		if (m_bRightPress)
			rotateNode(pos);
		else
			translasteNode(pos - m_lastPos);
		m_lastPos = pos;
		m_stage->Refresh();
	}

	return false;
}

bool EditMeshOP::onDraw() const
{
	if (Shape* shape = m_stage->GetShape())
	{
		shape->DrawTexture(d2d::Matrix());
		shape->DrawInfoXY();
	}

	d2d::PrimitiveDraw::cross(m_center, CENTER_EDGE, d2d::Colorf(0.2f, 0.8f, 0.4f));
	d2d::PrimitiveDraw::drawCircle(m_center, CENTER_RADIUS, true, 2, d2d::Colorf(0.2f, 0.4f, 0.8f));

	if (SelectNodesOP::onDraw())
		return true;

	return false;
}

void EditMeshOP::translasteNode(const d2d::Vector& offset)
{
	std::vector<Node*> nodes;
	m_selection.Traverse(d2d::FetchAllVisitor<Node>(nodes));
	for (int i = 0, n = nodes.size(); i < n; ++i)
	{
		nodes[i]->xy += offset;
	}
}

void EditMeshOP::rotateNode(const d2d::Vector& dst)
{
	float angle = d2d::Math::getAngleInDirection(m_center, m_lastPos, dst);
	std::vector<Node*> nodes;
	m_selection.Traverse(d2d::FetchAllVisitor<Node>(nodes));
	for (int i = 0, n = nodes.size(); i < n; ++i)
	{
		Node* node = nodes[i];
		d2d::Vector v = node->xy - m_center;
		v = d2d::Math::rotateVector(v, angle);
		node->xy = m_center + v;
	}
}

}