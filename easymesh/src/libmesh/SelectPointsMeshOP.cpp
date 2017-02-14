#include "SelectPointsMeshOP.h"
#include "StagePanel.h"
#include "PointsMesh.h"
#include "color_config.h"

#include <ee/panel_msg.h>
#include <ee/FetchAllVisitor.h>
#include <ee/EditPanelImpl.h>

#include <polymesh/Mesh.h>
#include <sprite2/S2_RVG.h>

namespace emesh
{

SelectPointsMeshOP::SelectPointsMeshOP(StagePanel* stage)
	: ee::DrawRectangleOP(stage, stage->GetStageImpl(), false)
	, m_draggable(true)
{
	m_first_pos.MakeInvalid();
}

bool SelectPointsMeshOP::OnMouseLeftDown(int x, int y)
{
	if (ee::DrawRectangleOP::OnMouseLeftDown(x, y)) 
		return true;

	Mesh* mesh = static_cast<StagePanel*>(m_wnd)->GetMesh();
	if (!mesh) return false;

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	int selected = mesh->PointQueryVertex(pos);
	if (selected != -1)
	{
		std::set<int>::iterator itr = m_selection.find(selected);
		if (m_stage->GetKeyState(WXK_CONTROL))
		{
			if (itr != m_selection.end()) {
				m_selection.erase(itr);
			} else {
				m_selection.insert(selected);
			}
		}
		else
		{
			if (itr == m_selection.end()) {
				m_selection.clear();
				m_selection.insert(selected);
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
			m_selection.clear();
			ee::SetCanvasDirtySJ::Instance()->SetDirty();
		}
	}

	return false;
}

bool SelectPointsMeshOP::OnMouseLeftUp(int x, int y)
{
	if (DrawRectangleOP::OnMouseLeftUp(x, y)) return true;

	m_draggable = true;

	Mesh* mesh = static_cast<StagePanel*>(m_wnd)->GetMesh();
	if (m_first_pos.IsValid() && mesh)
	{
		sm::vec2 end = m_stage->TransPosScrToProj(x, y);
		sm::rect rect(m_first_pos, end);
		std::vector<int> vertices;
		mesh->RectQueryVertices(rect, vertices);
		for (size_t i = 0, n = vertices.size(); i < n; ++i) {
			m_selection.insert(vertices[i]);
		}

		m_first_pos.MakeInvalid();

		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}

	//	enableRightTap(m_selection.empty());

	return false;
}

bool SelectPointsMeshOP::OnMouseDrag(int x, int y)
{
	if (ee::DrawRectangleOP::OnMouseDrag(x, y)) return true;

	return !m_draggable;
}

bool SelectPointsMeshOP::OnDraw() const
{
	if (m_first_pos.IsValid())
	{
		if (ee::DrawRectangleOP::OnDraw())
			return true;
	}

	if (Mesh* mesh = static_cast<StagePanel*>(m_wnd)->GetMesh()) 
	{
		std::vector<sm::vec2> vertices, texcoords;
		std::vector<int> triangles;
		mesh->GetMesh()->Dump(vertices, texcoords, triangles);

		s2::RVG::SetColor(GREEN);
		std::set<int>::const_iterator itr = m_selection.begin();
		for ( ; itr != m_selection.end(); ++itr) {
			s2::RVG::Circle(vertices[*itr], mesh->GetNodeRadius(), true);	
		}
	}

	return false;
}

bool SelectPointsMeshOP::Clear()
{
	if (ee::DrawRectangleOP::Clear()) return true;

 	m_selection.clear();
	m_first_pos.MakeInvalid();

	return false;
}

}