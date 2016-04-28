#include "EditNWOP.h"
#include "StagePanel.h"
#include "Triangle.h"
#include "Mesh.h"
#include "MeshRenderer.h"

#include <ee/Math2D.h>
#include <ee/panel_msg.h>
#include <ee/Matrix.h>
#include <ee/EE_RVG.h>
#include <ee/FetchAllVisitor.h>
#include <ee/EditPanelImpl.h>
#include <ee/RenderParams.h>

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
	m_last_pos.SetInvalid();
	m_center.Set(0, 0);
}

bool EditNWOP::OnMouseLeftDown(int x, int y)
{
	m_dragable = true;

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
		ee::Vector pos = m_stage->TransPosScrToProj(x, y);
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
		ee::Vector pos = m_stage->TransPosScrToProj(x, y);
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
		MeshRenderer::DrawTexture(mesh, ee::RenderParams());
		MeshRenderer::DrawInfoXY(mesh);
	}

	ee::RVG::Color(ee::Colorf(0.2f, 0.8f, 0.4f));
	ee::RVG::Cross(m_center, CENTER_EDGE);
	ee::RVG::Color(ee::Colorf(0.2f, 0.4f, 0.8f));
	ee::RVG::Circle(m_center, CENTER_RADIUS, true);

	if (SelectNodesOP::OnDraw())
		return true;

	return false;
}

void EditNWOP::TranslasteNode(const ee::Vector& offset)
{
	std::vector<Node*> nodes;
	m_selection.Traverse(ee::FetchAllVisitor<Node>(nodes));
	for (int i = 0, n = nodes.size(); i < n; ++i)
	{
		nodes[i]->xy += offset;
	}
}

void EditNWOP::RotateNode(const ee::Vector& dst)
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