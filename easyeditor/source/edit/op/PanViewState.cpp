#include "PanViewState.h"
#include "EditPanelImpl.h"
#include "panel_msg.h"
#include "OrthoCamera.h"
#include "Pseudo3DCamera.h"
#include "CameraMgr.h"

#include <gum/RenderContext.h>

namespace ee
{

PanViewState::PanViewState(EditPanelImpl* stage)
	: m_stage(stage)
{
	m_last_pos.MakeInvalid();
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
	m_last_pos.MakeInvalid();
	return false;
}

bool PanViewState::OnMouseDrag(int x, int y)
{
	if (!m_last_pos.IsValid()) {
		return false;
	}

	sm::vec2 offset(m_last_pos.x - x, m_last_pos.y - y);
	offset.y = -offset.y;
	if (CameraMgr::Instance()->IsType(CameraMgr::ORTHO)) {
		OrthoCamera* cam = static_cast<OrthoCamera*>(CameraMgr::Instance()->GetCamera());
		cam->Translate(offset);
	} else if (CameraMgr::Instance()->IsType(CameraMgr::PSEUDO3D)) {
		Pseudo3DCamera* cam = static_cast<Pseudo3DCamera*>(CameraMgr::Instance()->GetCamera());		
		int w = gum::RenderContext::Instance()->GetWidth(),
			h = gum::RenderContext::Instance()->GetHeight();		
		sm::vec2 last = cam->TransPosScreenToProject(m_last_pos.x, m_last_pos.y, w, h);
		sm::vec2 curr = cam->TransPosScreenToProject(x, y, w, h);
		cam->Translate(sm::vec3(curr.x - last.x, curr.y - last.y, 0));
	}

	m_last_pos.Set(x, y);

	SetCanvasDirtySJ::Instance()->SetDirty();

	return false;
}

}