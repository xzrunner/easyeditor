#include "PanViewState.h"
#include "EditPanelImpl.h"
#include "panel_msg.h"
#include "OrthoCamera.h"
#include "CameraMgr.h"

namespace ee
{

PanViewState::PanViewState(EditPanelImpl* stage)
	: m_stage(stage)
{
	m_last_pos.SetInvalid();
}

PanViewState::~PanViewState()
{
}

void PanViewState::Bind()
{
	m_stage->SetCursor(wxCURSOR_HAND);
}

void PanViewState::UnBind()
{
	m_stage->SetCursor(wxCURSOR_ARROW);
}

bool PanViewState::OnMousePress(int x, int y)
{
	m_last_pos.Set(x, y);
	return false;
}

bool PanViewState::OnMouseRelease(int x, int y)
{
	m_last_pos.SetInvalid();
	return false;
}

bool PanViewState::OnMouseDrag(int x, int y)
{
	if (m_last_pos.IsValid() && CameraMgr::Instance()->IsType(CameraMgr::ORTHO)) 
	{
		Vector offset(m_last_pos.x - x, m_last_pos.y - y);
		offset.y = -offset.y;
		static_cast<OrthoCamera*>(CameraMgr::Instance()->GetCamera())->Translate(offset);

		m_last_pos.Set(x, y);

		SetCanvasDirtySJ::Instance()->SetDirty();
	}
	return false;
}

}