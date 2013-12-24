#include "EditPartSkeletonOP.h"
#include "tools/Render.h"

namespace eanim
{

EditPartSkeletonOP::EditPartSkeletonOP(d2d::EditPanel* editPanel, PartSkeleton* skeleton)
	: d2d::ZoomViewOP(editPanel, false)
{
	m_skeleton = skeleton;
	m_selectedJoint = NULL;
	m_selectedNode = NULL;
}

bool EditPartSkeletonOP::onMouseLeftDown(int x, int y)
{
	if (ZoomViewOP::onMouseLeftDown(x, y)) return true;

	d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
	m_selectedJoint = m_skeleton->queryJointByPos(pos);
	if (!m_selectedJoint) 
	{
		m_selectedNode = m_skeleton->queryNodeByPos(pos);
		m_skeleton->translateNode(m_selectedNode, pos);
	}
	m_editPanel->Refresh();

	return false;
}

bool EditPartSkeletonOP::onMouseLeftUp(int x, int y)
{
	if (ZoomViewOP::onMouseLeftUp(x, y)) return true;

	m_selectedJoint = NULL;
	m_selectedNode = NULL;

	return false;
}

bool EditPartSkeletonOP::onMouseDrag(int x, int y)
{
	if (ZoomViewOP::onMouseDrag(x, y)) return true;

	if (m_selectedJoint)
	{
		d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
		m_skeleton->translateJoint(m_selectedJoint, pos);
		m_editPanel->Refresh();
	}
	else if (m_selectedNode)
	{
		d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
		m_skeleton->translateNode(m_selectedNode, pos);
		m_editPanel->Refresh();
	}

	return false;
}

bool EditPartSkeletonOP::onDraw() const
{
	if (ZoomViewOP::onDraw()) return true;

	m_skeleton->onDraw();

	return false;
}

bool EditPartSkeletonOP::clear()
{
	if (ZoomViewOP::clear()) return true;

	m_selectedJoint = NULL;
	m_selectedNode = NULL;

	return false;
}

} // eanim