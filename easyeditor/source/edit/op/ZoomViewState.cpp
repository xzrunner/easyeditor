#include "ZoomViewState.h"
#include "EditPanelImpl.h"
#include "panel_msg.h"

namespace ee
{

ZoomViewState::ZoomViewState(EditPanelImpl* stage)
	: m_stage(stage)
{
}

ZoomViewState::~ZoomViewState()
{
}

bool ZoomViewState::OnMouseWheelRotation(int x, int y, int direction)
{
	m_stage->OnMouseWheelRotation(x, y, direction);
	SetCanvasDirtySJ::Instance()->SetDirty();
	return true;
}

}