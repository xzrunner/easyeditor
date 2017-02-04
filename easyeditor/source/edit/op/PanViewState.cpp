#include "PanViewState.h"
#include "EditPanelImpl.h"
#include "panel_msg.h"
#include "CameraCanvas.h"

#include <gum/RenderContext.h>
#include <sprite2/OrthoCamera.h>
#include <sprite2/Pseudo3DCamera.h>

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

	ee::CameraCanvas* canvas = dynamic_cast<ee::CameraCanvas*>(m_stage->GetCanvas());
	if (canvas)
	{
		switch (canvas->GetCamera()->Type())
		{
		case s2::CAM_ORTHO2D:
			{
				s2::OrthoCamera* cam = static_cast<s2::OrthoCamera*>(canvas->GetCamera());
				cam->Translate(offset);
			}
			break;
		case s2::CAM_PSEUDO3D:
			{
				s2::Pseudo3DCamera* cam = static_cast<s2::Pseudo3DCamera*>(canvas->GetCamera());		
				int w = gum::RenderContext::Instance()->GetWidth(),
					h = gum::RenderContext::Instance()->GetHeight();		
				sm::vec2 last = cam->TransPosScreenToProject(m_last_pos.x, m_last_pos.y, w, h);
				sm::vec2 curr = cam->TransPosScreenToProject(x, y, w, h);
				cam->Translate(sm::vec3(curr.x - last.x, curr.y - last.y, 0));
			}
			break;
		}
	}

	m_last_pos.Set(x, y);

	SetCanvasDirtySJ::Instance()->SetDirty();

	return false;
}

}