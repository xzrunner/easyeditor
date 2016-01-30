#include "SelectNodesOP.h"
#include "StagePanel.h"
#include "Mesh.h"

namespace emesh
{

SelectNodesOP::SelectNodesOP(StagePanel* stage)
	: d2d::DrawRectangleOP(stage, false)
	, m_stage(stage)
	, m_bDraggable(true)
{
	m_style.color = d2d::Colorf(0.8f, 0.2f, 0.2f);

	m_first_pos.SetInvalid();
}

SelectNodesOP::~SelectNodesOP()
{
	m_selection.Clear();
}

bool SelectNodesOP::OnMouseLeftDown(int x, int y)
{
	if (d2d::DrawRectangleOP::OnMouseLeftDown(x, y)) 
		return true;

	Shape* shape = m_stage->GetShape();
	if (!shape) return false;

	d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
	std::vector<Node*> nodes;
	shape->QueryNode(pos, nodes);
	if (!nodes.empty())
	{
		m_selection.Clear();
		for (int i = 0, n = nodes.size(); i < n; ++i)
		{
			Node* node = nodes[i];
			if (m_stage->GetKeyState(WXK_CONTROL)) {
				if (m_selection.IsExist(node))
					m_selection.Remove(node);
				else
					m_selection.Add(node);
			} else {
				if (!m_selection.IsExist(node)) {
					m_selection.Add(node);
				}
			}
			m_first_pos.SetInvalid();
		}
	}
	else
	{
		DrawRectangleOP::OnMouseLeftDown(x, y);
		m_first_pos = pos;
		if (m_stage->GetKeyState(WXK_CONTROL))
			m_bDraggable = false;
		else
			m_selection.Clear();
		d2d::SetCanvasDirtySJ::Instance()->SetDirty();
	}

	return false;
}

bool SelectNodesOP::OnMouseLeftUp(int x, int y)
{
	if (DrawRectangleOP::OnMouseLeftUp(x, y)) return true;

	m_bDraggable = true;

	Shape* shape = m_stage->GetShape();
	if (m_first_pos.IsValid() && shape)
	{
		d2d::Vector end = m_stage->TransPosScrToProj(x, y);
		d2d::Rect rect(m_first_pos, end);
		std::vector<Node*> nodes;
		shape->QueryNode(rect, nodes);
		for (size_t i = 0, n = nodes.size(); i < n; ++i)
			m_selection.Add(nodes[i]);

		m_first_pos.SetInvalid();
	}

	//	enableRightTap(m_selection->empty());

	return false;
}

bool SelectNodesOP::OnMouseDrag(int x, int y)
{
	if (d2d::DrawRectangleOP::OnMouseDrag(x, y)) return true;

	return !m_bDraggable;
}

bool SelectNodesOP::OnDraw() const
{
	if (m_first_pos.IsValid())
	{
		if (d2d::DrawRectangleOP::OnDraw())
			return true;
	}

	std::vector<Node*> nodes;
	m_selection.Traverse(d2d::FetchAllVisitor<Node>(nodes));
	std::vector<d2d::Vector> points;
	points.reserve(nodes.size());
	for (int i = 0, n = nodes.size(); i < n; ++i)
		points.push_back(nodes[i]->xy);

	if (Shape* shape = m_stage->GetShape()) {
		d2d::PrimitiveDraw::DrawCircles(points, shape->GetNodeRegion(), 
			true, 2, d2d::Colorf(0.4f, 0.8f, 0.2f, 0.5f));
	}

	return false;
}

bool SelectNodesOP::Clear()
{
	if (d2d::DrawRectangleOP::Clear()) return true;

	m_selection.Clear();
	m_first_pos.SetInvalid();

	return false;
}

}