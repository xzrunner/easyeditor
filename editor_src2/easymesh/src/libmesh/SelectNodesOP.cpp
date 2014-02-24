#include "SelectNodesOP.h"
#include "StagePanel.h"
#include "Mesh.h"

namespace emesh
{

SelectNodesOP::SelectNodesOP(StagePanel* stage)
: d2d::DrawRectangleOP(stage, d2d::Colorf(0.8f, 0.2f, 0.2f))
	, m_stage(stage)
	, m_bDraggable(true)
{
	m_firstPos.setInvalid();
}

SelectNodesOP::~SelectNodesOP()
{
	m_selection.clear();
}

bool SelectNodesOP::onMouseLeftDown(int x, int y)
{
	if (d2d::DrawRectangleOP::onMouseLeftDown(x, y)) 
		return true;

	Mesh* mesh = m_stage->getMesh();
	if (!mesh) return false;

	d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
	Node* selected = mesh->queryNode(pos);
	if (selected)
	{
		if (wxGetKeyState(WXK_CONTROL))
		{
			if (m_selection.isExist(selected))
				m_selection.erase(selected);
			else
				m_selection.insert(selected);
		}
		else
		{
			if (!m_selection.isExist(selected))
			{
				m_selection.clear();
				m_selection.insert(selected);
			}
		}
		m_firstPos.setInvalid();
	}
	else
	{
		DrawRectangleOP::onMouseLeftDown(x, y);
		m_firstPos = pos;
		if (wxGetKeyState(WXK_CONTROL))
			m_bDraggable = false;
		else
			m_selection.clear();
		m_editPanel->Refresh();
	}

	return false;
}

bool SelectNodesOP::onMouseLeftUp(int x, int y)
{
	if (DrawRectangleOP::onMouseLeftUp(x, y)) return true;

	m_bDraggable = true;

	Mesh* mesh = m_stage->getMesh();
	if (m_firstPos.isValid() && mesh)
	{
		d2d::Vector end = m_editPanel->transPosScreenToProject(x, y);
		d2d::Rect rect(m_firstPos, end);
		std::vector<Node*> nodes;
		mesh->queryNode(rect, nodes);
		for (size_t i = 0, n = nodes.size(); i < n; ++i)
			m_selection.insert(nodes[i]);

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
	m_selection.traverse(d2d::FetchAllVisitor<Node>(nodes));
	std::vector<d2d::Vector> points;
	points.reserve(nodes.size());
	for (int i = 0, n = nodes.size(); i < n; ++i)
		points.push_back(nodes[i]->xy);

	d2d::PrimitiveDraw::drawCircles(points, Node::RADIUS, true, 2, d2d::Colorf(0.4f, 0.8f, 0.2f, 0.5f));

	return false;
}

bool SelectNodesOP::clear()
{
	if (d2d::DrawRectangleOP::clear()) return true;

	m_selection.clear();
	m_firstPos.setInvalid();

	return false;
}

}