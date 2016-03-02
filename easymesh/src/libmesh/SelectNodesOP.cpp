#include "SelectNodesOP.h"
#include "StagePanel.h"
#include "Mesh.h"

#include <ee/panel_msg.h>
#include <ee/FetchAllVisitor.h>
#include <ee/EE_RVG.h>
#include <ee/EditPanelImpl.h>

namespace emesh
{

SelectNodesOP::SelectNodesOP(StagePanel* stage)
	: ee::DrawRectangleOP(stage, stage->GetStageImpl(), false)
	, m_draggable(true)
{
	m_style.color = ee::Colorf(0.8f, 0.2f, 0.2f);

	m_first_pos.SetInvalid();
}

SelectNodesOP::~SelectNodesOP()
{
	m_selection.Clear();
}

bool SelectNodesOP::OnMouseLeftDown(int x, int y)
{
	if (ee::DrawRectangleOP::OnMouseLeftDown(x, y)) 
		return true;

	Shape* shape = static_cast<StagePanel*>(m_wnd)->GetShape();
	if (!shape) return false;

	ee::Vector pos = m_stage->TransPosScrToProj(x, y);
	Node* selected = shape->PointQueryNode(pos);
	if (selected)
	{
		if (m_stage->GetKeyState(WXK_CONTROL))
		{
			if (m_selection.IsExist(selected)) {
				m_selection.Remove(selected);
			} else {
				m_selection.Add(selected);
			}
		}
		else
		{
			if (!m_selection.IsExist(selected)) {
				m_selection.Add(selected);
			}
		}
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}
	else
	{
		DrawRectangleOP::OnMouseLeftDown(x, y);

		m_first_pos = pos;
		if (m_stage->GetKeyState(WXK_CONTROL)) {
			m_draggable = false;
		} else {
			m_selection.Clear();
			ee::SetCanvasDirtySJ::Instance()->SetDirty();
		}
	}

	return false;
}

bool SelectNodesOP::OnMouseLeftUp(int x, int y)
{
	if (DrawRectangleOP::OnMouseLeftUp(x, y)) return true;

	m_draggable = true;

	Shape* shape = static_cast<StagePanel*>(m_wnd)->GetShape();
	if (m_first_pos.IsValid() && shape)
	{
		ee::Vector end = m_stage->TransPosScrToProj(x, y);
		ee::Rect rect(m_first_pos, end);
		std::vector<Node*> nodes;
		shape->RectQueryNodes(rect, nodes);
		for (size_t i = 0, n = nodes.size(); i < n; ++i) {
			m_selection.Add(nodes[i]);
		}

		m_first_pos.SetInvalid();

		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}

	//	enableRightTap(m_selection.empty());

	return false;
}

bool SelectNodesOP::OnMouseDrag(int x, int y)
{
	if (ee::DrawRectangleOP::OnMouseDrag(x, y)) return true;

	return !m_draggable;
}

bool SelectNodesOP::OnDraw() const
{
	if (m_first_pos.IsValid())
	{
		if (ee::DrawRectangleOP::OnDraw())
			return true;
	}

	std::vector<Node*> nodes;
	m_selection.Traverse(ee::FetchAllVisitor<Node>(nodes));
	std::vector<ee::Vector> points;
	points.reserve(nodes.size());
	for (int i = 0, n = nodes.size(); i < n; ++i)
		points.push_back(nodes[i]->xy);

	if (Shape* shape = static_cast<StagePanel*>(m_wnd)->GetShape()) {
		ee::RVG::Circles(points, shape->GetNodeRegion(), true, ee::Colorf(0.4f, 0.8f, 0.2f, 0.5f));
	}

	return false;
}

bool SelectNodesOP::Clear()
{
	if (ee::DrawRectangleOP::Clear()) return true;

	m_selection.Clear();
	m_first_pos.SetInvalid();

	return false;
}

}