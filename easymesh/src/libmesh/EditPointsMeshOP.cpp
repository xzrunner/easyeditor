#include "EditPointsMeshOP.h"
#include "StagePanel.h"
#include "Mesh.h"

#include <ee/Math2D.h>
#include <ee/panel_msg.h>
#include <ee/FetchAllVisitor.h>
#include <ee/EditPanelImpl.h>

#include <polymesh/Mesh.h>
#include <sprite2/RenderParams.h>
#include <sprite2/S2_RVG.h>
#include <sprite2/DrawMesh.h>
#include <SM_Calc.h>

namespace emesh
{

static const int CENTER_RADIUS = 3;
static const int CENTER_EDGE = 10;

EditPointsMeshOP::EditPointsMeshOP(StagePanel* stage)
	: SelectPointsMeshOP(stage)
	, m_right_press(false)
	, m_select_center(false)
	, m_dragable(false)
{
	m_center.Set(0, 0);
}

bool EditPointsMeshOP::OnMouseLeftDown(int x, int y)
{
	m_dragable = true;

	m_last_pos = m_stage->TransPosScrToProj(x, y);
	if (sm::dis_pos_to_pos(m_last_pos, m_center) < CENTER_RADIUS)
	{
		m_select_center = true;
		return false;
	}

	if (SelectPointsMeshOP::OnMouseLeftDown(x, y))
		return true;

	m_right_press = false;

	return false;
}

bool EditPointsMeshOP::OnMouseLeftUp(int x, int y)
{
	m_dragable = false;

	if (SelectPointsMeshOP::OnMouseLeftUp(x, y))
		return true;

	m_select_center = false;

	return false;
}

bool EditPointsMeshOP::OnMouseRightDown(int x, int y)
{
	m_dragable = true;

	if (SelectPointsMeshOP::OnMouseRightDown(x, y))
		return true;

	m_last_pos = m_stage->TransPosScrToProj(x, y);

	m_right_press = true;

	return false;
}

bool EditPointsMeshOP::OnMouseRightUp(int x, int y)
{
	m_dragable = false;

	if (SelectPointsMeshOP::OnMouseRightUp(x, y))
		return true;

	return false;
}

bool EditPointsMeshOP::OnMouseDrag(int x, int y)
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

	if (SelectPointsMeshOP::OnMouseDrag(x, y))
		return true;

	Mesh* mesh = static_cast<StagePanel*>(m_wnd)->GetMesh();
	if (!mesh) return false;

	if (!m_selection.empty())
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

bool EditPointsMeshOP::OnDraw() const
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

	if (SelectPointsMeshOP::OnDraw())
		return true;

	return false;
}

void EditPointsMeshOP::TranslasteNode(const sm::vec2& offset)
{
	Mesh* mesh = static_cast<StagePanel*>(m_wnd)->GetMesh();
	if (!mesh) {
		return;
	}

	pm::Mesh* pm_mesh = mesh->GetMesh();

	std::vector<sm::vec2> vertices, texcoords;
	std::vector<int> triangles;
	pm_mesh->Dump(vertices, texcoords, triangles);
	std::set<int>::iterator itr = m_selection.begin();
	for ( ; itr != m_selection.end(); ++itr) {
		int idx = *itr;
		pm_mesh->SetVertexPos(idx, vertices[idx] + offset);
	}
}

void EditPointsMeshOP::RotateNode(const sm::vec2& dst)
{
	Mesh* mesh = static_cast<StagePanel*>(m_wnd)->GetMesh();
	if (!mesh) {
		return;
	}

	pm::Mesh* pm_mesh = mesh->GetMesh();

	float angle = sm::get_angle_in_direction(m_center, m_last_pos, dst);
	std::vector<sm::vec2> vertices, texcoords;
	std::vector<int> triangles;
	pm_mesh->Dump(vertices, texcoords, triangles);
	std::set<int>::iterator itr = m_selection.begin();
	for ( ; itr != m_selection.end(); ++itr) 
	{
		int idx = *itr;

		sm::vec2 v = vertices[idx] - m_center;
		v = sm::rotate_vector(v, angle);
		pm_mesh->SetVertexPos(idx, m_center + v);
	}
}

}