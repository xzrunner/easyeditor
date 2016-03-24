#include "EditMeshOP.h"
#include "StagePanel.h"
#include "Triangle.h"
#include "Shape.h"

#include <ee/Math2D.h>
#include <ee/panel_msg.h>
#include <ee/Matrix.h>
#include <ee/EE_RVG.h>
#include <ee/FetchAllVisitor.h>
#include <ee/EditPanelImpl.h>
#include <ee/SpriteTrans.h>

namespace emesh
{

static const int CENTER_RADIUS = 3;
static const int CENTER_EDGE = 10;

EditMeshOP::EditMeshOP(StagePanel* stage)
	: SelectNodesOP(stage)
	, m_right_press(false)
	, m_select_center(false)
{
	m_last_pos.SetInvalid();
	m_center.Set(0, 0);
}

bool EditMeshOP::OnMouseLeftDown(int x, int y)
{
	m_last_pos = m_stage->TransPosScrToProj(x, y);
	if (ee::Math2D::GetDistance(m_last_pos, m_center) < CENTER_RADIUS)
	{
		m_select_center = true;
		return false;
	}

	if (SelectNodesOP::OnMouseLeftDown(x, y))
		return true;

	m_right_press = false;

	return false;
}

bool EditMeshOP::OnMouseLeftUp(int x, int y)
{
	if (SelectNodesOP::OnMouseLeftUp(x, y))
		return true;

	m_select_center = false;

	return false;
}

bool EditMeshOP::OnMouseRightDown(int x, int y)
{
	if (SelectNodesOP::OnMouseRightDown(x, y))
		return true;

	m_last_pos = m_stage->TransPosScrToProj(x, y);

	m_right_press = true;

	return false;
}

bool EditMeshOP::OnMouseDrag(int x, int y)
{
	if (m_select_center)
	{
		ee::Vector pos = m_stage->TransPosScrToProj(x, y);
		m_center = pos;
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
		return true;
	}

	if (SelectNodesOP::OnMouseDrag(x, y))
		return true;

	Shape* shape = static_cast<StagePanel*>(m_wnd)->GetShape();
	if (!shape) return false;

	if (!m_selection.IsEmpty())
	{
		ee::Vector pos = m_stage->TransPosScrToProj(x, y);
		if (m_right_press)
			RotateNode(pos);
		else
			TranslasteNode(pos - m_last_pos);
		m_last_pos = pos;
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}

	return false;
}

bool EditMeshOP::OnDraw() const
{
	if (Shape* shape = static_cast<StagePanel*>(m_wnd)->GetShape())
	{
		shape->DrawTexture(ee::SpriteTrans());
		shape->DrawInfoXY();
	}

	ee::RVG::Color(ee::Colorf(0.2f, 0.8f, 0.4f));
	ee::RVG::Cross(m_center, CENTER_EDGE);
	ee::RVG::Color(ee::Colorf(0.2f, 0.4f, 0.8f));
	ee::RVG::Circle(m_center, CENTER_RADIUS, true);

	if (SelectNodesOP::OnDraw())
		return true;

	return false;
}

void EditMeshOP::TranslasteNode(const ee::Vector& offset)
{
	std::vector<Node*> nodes;
	m_selection.Traverse(ee::FetchAllVisitor<Node>(nodes));
	for (int i = 0, n = nodes.size(); i < n; ++i)
	{
		nodes[i]->xy += offset;
	}
}

void EditMeshOP::RotateNode(const ee::Vector& dst)
{
	float angle = ee::Math2D::GetAngleInDirection(m_center, m_last_pos, dst);
	std::vector<Node*> nodes;
	m_selection.Traverse(ee::FetchAllVisitor<Node>(nodes));
	for (int i = 0, n = nodes.size(); i < n; ++i)
	{
		Node* node = nodes[i];
		ee::Vector v = node->xy - m_center;
		v = ee::Math2D::RotateVector(v, angle);
		node->xy = m_center + v;
	}
}

}