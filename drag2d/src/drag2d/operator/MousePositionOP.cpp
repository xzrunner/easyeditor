#include "MousePositionOP.h"

#include "component/MousePositionCMPT.h"
#include "view/EditPanelImpl.h"

namespace d2d
{

MousePositionOP::MousePositionOP(wxWindow* wnd, EditPanelImpl* stage, 
								 MousePositionCMPT* cmpt)
	: ZoomViewOP(wnd, stage, true)
	, m_cmpt(cmpt)
{
}

bool MousePositionOP::OnMouseMove(int x, int y)
{
	if (ZoomViewOP::OnMouseMove(x, y)) return true;

	m_cmpt->updatePosition(m_stage->TransPosScrToProj(x, y));

	return false;
}

}