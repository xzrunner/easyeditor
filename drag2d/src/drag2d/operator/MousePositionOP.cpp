#include "MousePositionOP.h"

#include "component/MousePositionCMPT.h"

namespace d2d
{

MousePositionOP::MousePositionOP(EditPanel* editPanel, MousePositionCMPT* cmpt)
	: ZoomViewOP(editPanel, true)
	, m_cmpt(cmpt)
{
}

bool MousePositionOP::onMouseMove(int x, int y)
{
	if (ZoomViewOP::onMouseMove(x, y)) return true;

	m_cmpt->updatePosition(m_stage->transPosScreenToProject(x, y));

	return false;
}

} // d2d