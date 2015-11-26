#include "ZoomViewState.h"

#include "view/EditPanelImpl.h"
#include "message/SetCanvasDirtySJ.h"

namespace d2d
{

ZoomViewState::ZoomViewState(EditPanelImpl* stage)
	: m_stage(stage)
{
	m_stage->Retain();
}

ZoomViewState::~ZoomViewState()
{
	m_stage->Release();
}

bool ZoomViewState::OnMouseWheelRotation(int x, int y, int direction)
{
	m_stage->OnMouseWheelRotation(x, y, direction);
	SetCanvasDirtySJ::Instance()->SetDirty();
	return true;
}

}