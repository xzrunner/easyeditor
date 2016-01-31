#include "AddPartSkeletonJointOP.h"

#include "tools/Render.h"

namespace eanim
{

AddPartSkeletonJointOP::AddPartSkeletonJointOP(ee::EditPanel* editPanel, PartSkeleton* skeleton)
	: ee::ZoomViewOP(editPanel, false)
	, m_skeleton(skeleton)
{
	m_lastPos.setInvalid();
}

bool AddPartSkeletonJointOP::onMouseLeftDown(int x, int y)
{
	if (ZoomViewOP::onMouseLeftDown(x, y)) return true;

	m_lastPos = m_editPanel->transPosScreenToProject(x, y);
	m_skeleton->pushJoint(m_lastPos);
	m_editPanel->Refresh();

	return false;
}

bool AddPartSkeletonJointOP::onMouseRightDown(int x, int y)
{
	if (ZoomViewOP::onMouseRightDown(x, y)) return true;

	m_skeleton->popJoint();
	m_editPanel->Refresh();

	return false;
}

bool AddPartSkeletonJointOP::onMouseMove(int x, int y)
{
	if (ZoomViewOP::onMouseMove(x, y)) return true;

	m_lastPos = m_editPanel->transPosScreenToProject(x, y);
	m_editPanel->Refresh();

	return false;
}

bool AddPartSkeletonJointOP::onDraw() const
{
	if (ZoomViewOP::onDraw()) return true;

	m_skeleton->onDraw();

	ee::Vector last = m_skeleton->topJoint();
	if (last.isValid())
	{
		glLineWidth(2.0f);
		glColor3f(1.0f, 0.0f, 0.0f);

		glBegin(GL_LINES);
			glVertex2f(last.x, last.y);
			glVertex2f(m_lastPos.x, m_lastPos.y);
		glEnd();

		glLineWidth(1.0f);
	}

	return false;
}

bool AddPartSkeletonJointOP::clear()
{
	if (ZoomViewOP::clear()) return true;

	m_lastPos.setInvalid();

	return false;
}

} // eanim