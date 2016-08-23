#include "SelectNodesOP.h"
#include "StagePanel.h"
#include "Network.h"
#include "color_config.h"

#include <ee/panel_msg.h>
#include <ee/FetchAllVisitor.h>
#include <ee/EditPanelImpl.h>

#include <sprite2/S2_RVG.h>
#include <sprite2/MeshNode.h>

namespace emesh
{

SelectNodesOP::SelectNodesOP(StagePanel* stage)
	: ee::DrawRectangleOP(stage, stage->GetStageImpl(), false)
	, m_draggable(true)
{
	m_first_pos.MakeInvalid();
}

SelectNodesOP::~SelectNodesOP()
{
	m_selection.Clear();
}

bool SelectNodesOP::OnMouseLeftDown(int x, int y)
{
	if (ee::DrawRectangleOP::OnMouseLeftDown(x, y)) 
		return true;

	Mesh* mesh = static_cast<StagePanel*>(m_wnd)->GetMesh();
	if (!mesh) return false;

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	s2::MeshNode* selected = mesh->PointQueryNode(pos);
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
				m_selection.Clear();
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

	Mesh* mesh = static_cast<StagePanel*>(m_wnd)->GetMesh();
	if (m_first_pos.IsValid() && mesh)
	{
		sm::vec2 end = m_stage->TransPosScrToProj(x, y);
		sm::rect rect(m_first_pos, end);
		std::vector<s2::MeshNode*> nodes;
		mesh->RectQueryNodes(rect, nodes);
		for (size_t i = 0, n = nodes.size(); i < n; ++i) {
			m_selection.Add(nodes[i]);
		}

		m_first_pos.MakeInvalid();

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

	std::vector<s2::MeshNode*> nodes;
	m_selection.Traverse(ee::FetchAllVisitor<s2::MeshNode>(nodes));
	std::vector<sm::vec2> points;
	points.reserve(nodes.size());
	for (int i = 0, n = nodes.size(); i < n; ++i)
		points.push_back(nodes[i]->xy);

	if (Mesh* mesh = static_cast<StagePanel*>(m_wnd)->GetMesh()) {
		s2::RVG::SetColor(GREEN);
		s2::RVG::Circles(points, mesh->GetNodeRegion(), true);
	}

	return false;
}

bool SelectNodesOP::Clear()
{
	if (ee::DrawRectangleOP::Clear()) return true;

	m_selection.Clear();
	m_first_pos.MakeInvalid();

	return false;
}

}