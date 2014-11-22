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

	m_firstPos.setInvalid();
}

SelectNodesOP::~SelectNodesOP()
{
	m_selection.Clear();
}

bool SelectNodesOP::onMouseLeftDown(int x, int y)
{
	if (d2d::DrawRectangleOP::onMouseLeftDown(x, y)) 
		return true;

	Shape* shape = m_stage->GetShape();
	if (!shape) return false;

	d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
	std::vector<Node*> nodes;
	shape->QueryNode(pos, nodes);
	if (!nodes.empty())
	{
		m_selection.Clear();
		for (int i = 0, n = nodes.size(); i < n; ++i)
		{
			Node* node = nodes[i];
			if (wxGetKeyState(WXK_CONTROL)) {
				if (m_selection.IsExist(node))
					m_selection.Remove(node);
				else
					m_selection.Add(node);
			} else {
				if (!m_selection.IsExist(node)) {
					m_selection.Add(node);
				}
			}
			m_firstPos.setInvalid();
		}
	}
	else
	{
		DrawRectangleOP::onMouseLeftDown(x, y);
		m_firstPos = pos;
		if (wxGetKeyState(WXK_CONTROL))
			m_bDraggable = false;
		else
			m_selection.Clear();
		m_editPanel->Refresh();
	}

	return false;
}

bool SelectNodesOP::onMouseLeftUp(int x, int y)
{
	if (DrawRectangleOP::onMouseLeftUp(x, y)) return true;

	m_bDraggable = true;

	Shape* shape = m_stage->GetShape();
	if (m_firstPos.isValid() && shape)
	{
		d2d::Vector end = m_editPanel->transPosScreenToProject(x, y);
		d2d::Rect rect(m_firstPos, end);
		std::vector<Node*> nodes;
		shape->QueryNode(rect, nodes);
		for (size_t i = 0, n = nodes.size(); i < n; ++i)
			m_selection.Add(nodes[i]);

		m_firstPos.setInvalid();
	}

	//	enableRightTap(m_selection->empty());

	return false;
}

bool SelectNodesOP::onMouseDrag(int x, int y)
{
	if (d2d::DrawRectangleOP::onMouseDrag(x, y)) return true;

	return !m_bDraggable;
}

bool SelectNodesOP::onDraw() const
{
	if (m_firstPos.isValid())
	{
		if (d2d::DrawRectangleOP::onDraw())
			return true;
	}

	std::vector<Node*> nodes;
	m_selection.Traverse(d2d::FetchAllVisitor<Node>(nodes));
	std::vector<d2d::Vector> points;
	points.reserve(nodes.size());
	for (int i = 0, n = nodes.size(); i < n; ++i)
		points.push_back(nodes[i]->xy);

	if (Shape* shape = m_stage->GetShape()) {
		d2d::PrimitiveDraw::drawCircles(points, shape->GetNodeRegion(), 
			true, 2, d2d::Colorf(0.4f, 0.8f, 0.2f, 0.5f));
	}

	return false;
}

bool SelectNodesOP::clear()
{
	if (d2d::DrawRectangleOP::clear()) return true;

	m_selection.Clear();
	m_firstPos.setInvalid();

	return false;
}

}