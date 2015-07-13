#include "ZoomViewState.h"

#include "view/EditPanel.h"

namespace d2d
{

bool ZoomViewState::OnMouseWheelRotation(int x, int y, int direction)
{
	m_stage->OnMouseWheelRotation(x, y, direction);
	m_stage->SetCanvasDirty();
	return true;
}

}