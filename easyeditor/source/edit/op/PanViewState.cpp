#include "PanViewState.h"
#include "EditPanelImpl.h"
#include "panel_msg.h"
#include "OrthoCamera.h"
#include "Pseudo3DCamera.h"
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
	if (!m_last_pos.IsValid()) {
		return false;
	}

	Vector offset(m_last_pos.x - x, m_last_pos.y - y);
	offset.y = -offset.y;
	if (CameraMgr::Instance()->IsType(CameraMgr::ORTHO)) {
		static_cast<OrthoCamera*>(CameraMgr::Instance()->GetCamera())->Translate(offset);
	} else if (CameraMgr::Instance()->IsType(CameraMgr::PSEUDO3D)) {
		static_cast<Pseudo3DCamera*>(CameraMgr::Instance()->GetCamera())->Translate(vec3(-offset.x, -offset.y, 0));
	}

	m_last_pos.Set(x, y);

	SetCanvasDirtySJ::Instance()->SetDirty();

	return false;
}

}