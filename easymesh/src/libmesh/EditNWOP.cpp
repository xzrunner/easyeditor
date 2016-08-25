#include "EditNWOP.h"
#include "StagePanel.h"
#include "Mesh.h"

#include <ee/Math2D.h>
#include <ee/panel_msg.h>
#include <ee/FetchAllVisitor.h>
#include <ee/EditPanelImpl.h>

#include <sprite2/RenderParams.h>
#include <sprite2/S2_RVG.h>
#include <sprite2/MeshNode.h>
#include <sprite2/DrawMesh.h>
#include <SM_Calc.h>

namespace emesh
{

static const int CENTER_RADIUS = 3;
static const int CENTER_EDGE = 10;

EditNWOP::EditNWOP(StagePanel* stage)
	: SelectNodesOP(stage)
	, m_right_press(false)
	, m_select_center(false)
	, m_dragable(false)
{
	m_center.Set(0, 0);
}

bool EditNWOP::OnMouseLeftDown(int x, int y)
{
	m_dragable = true;

	m_last_pos = m_stage->TransPosScrToProj(x, y);
	if (sm::dis_pos_to_pos(m_last_pos, m_center) < CENTER_RADIUS)
	{
		m_select_center = true;
		return false;
	}

	if (SelectNodesOP::OnMouseLeftDown(x, y))
		return true;

	m_right_press = false;

	return false;
}

bool EditNWOP::OnMouseLeftUp(int x, int y)
{
	m_dragable = false;

	if (SelectNodesOP::OnMouseLeftUp(x, y))
		return true;

	m_select_center = false;

	return false;
}

bool EditNWOP::OnMouseRightDown(int x, int y)
{
	m_dragable = true;

	if (SelectNodesOP::OnMouseRightDown(x, y))
		return true;

	m_last_pos = m_stage->TransPosScrToProj(x, y);

	m_right_press = true;

	return false;
}

bool EditNWOP::OnMouseRightUp(int x, int y)
{
	m_dragable = false;

	if (SelectNodesOP::OnMouseRightUp(x, y))
		return true;

	return false;
}

bool EditNWOP::OnMouseDrag(int x, int y)
{
	if (!m_dragable) {
		return false;
	}

	if (m_select_center)
	{
		sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
		m_center = pos;
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
		return true;
	}

	if (SelectNodesOP::OnMouseDrag(x, y))
		return true;

	Mesh* mesh = static_cast<StagePanel*>(m_wnd)->GetMesh();
	if (!mesh) return false;

	if (!m_selection.IsEmpty())
	{
		sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
		if (m_right_press)
			RotateNode(pos);
		else
			TranslasteNode(pos - m_last_pos);
		m_last_pos = pos;
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
		ee::SetWndDirtySJ::Instance()->SetDirty();
	}

	return false;
}

bool EditNWOP::OnDraw() const
{
	if (Mesh* mesh = static_cast<StagePanel*>(m_wnd)->GetMesh())
	{
		s2::DrawMesh::DrawTexture(mesh, s2::RenderParams());
		s2::DrawMesh::DrawInfoXY(mesh);
	}

	s2::RVG::SetColor(s2::Color(51, 204, 51));
	s2::RVG::Cross(m_center, CENTER_EDGE);
	s2::RVG::SetColor(s2::Color(51, 102, 204));
	s2::RVG::Circle(m_center, CENTER_RADIUS, true);

	if (SelectNodesOP::OnDraw())
		return true;

	return false;
}

void EditNWOP::TranslasteNode(const sm::vec2& offset)
{
	std::vector<s2::MeshNode*> nodes;
	m_selection.Traverse(ee::FetchAllVisitor<s2::MeshNode>(nodes));
	for (int i = 0, n = nodes.size(); i < n; ++i)
	{
		nodes[i]->xy += offset;
	}
}

void EditNWOP::RotateNode(const sm::vec2& dst)
{
	float angle = sm::get_angle_in_direction(m_center, m_last_pos, dst);
	std::vector<s2::MeshNode*> nodes;
	m_selection.Traverse(ee::FetchAllVisitor<s2::MeshNode>(nodes));
	for (int i = 0, n = nodes.size(); i < n; ++i)
	{
		s2::MeshNode* node = nodes[i];
		sm::vec2 v = node->xy - m_center;
		v = sm::rotate_vector(v, angle);
		node->xy = m_center + v;
	}
}

}