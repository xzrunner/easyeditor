#include "MousePositionOP.h"
#include "MousePositionCMPT.h"
#include "EditPanelImpl.h"

namespace ee
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