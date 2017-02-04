#include "CtrlCamOP.h"

#include "frame/SettingCfg.h"

#include <ee/EditPanelImpl.h>
#include <ee/panel_msg.h>
#include <ee/CameraCanvas.h>

#include <sprite2/OrthoCamera.h>

namespace lr
{
namespace preview
{

CtrlCamOP::CtrlCamOP(wxWindow* wnd, ee::EditPanelImpl* stage)
	: ee::EditOP(wnd, stage)
{	
}

bool CtrlCamOP::OnKeyDown(int keyCode)
{
	SettingCfg* cfg = SettingCfg::Instance();

	switch (keyCode)
	{
	case 'a': case 'A':
		OffsetCamera(-cfg->m_view_width * 0.5f, 0.0f);
		break;
	case 'd': case 'D':
		OffsetCamera(cfg->m_view_width * 0.5f, 0.0f);
		break;
	case 's': case 'S':
		OffsetCamera(0.0f, -cfg->m_view_height * 0.5f);
		break;
	case 'w': case 'W':
		OffsetCamera(0.0f, cfg->m_view_height * 0.5f);
		break;
	}

	return false;
}

void CtrlCamOP::OffsetCamera(float dx, float dy)
{
	s2::Camera* cam = static_cast<ee::CameraCanvas*>(m_stage->GetCanvas())->GetCamera();
	if (cam->Type() == s2::CAM_ORTHO2D) {
		s2::OrthoCamera* ortho_cam = static_cast<s2::OrthoCamera*>(cam);
		ortho_cam->Translate(sm::vec2(dx, dy));
	}
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

}
}