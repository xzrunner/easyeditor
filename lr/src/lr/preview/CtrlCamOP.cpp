#include "CtrlCamOP.h"

#include "frame/SettingCfg.h"

#include <ee/EditPanelImpl.h>
#include <ee/panel_msg.h>
#include <ee/OrthoCamera.h>
#include <ee/CameraMgr.h>

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
	SettingCfg* cfg = SettingCfg::Instance();
	if (ee::CameraMgr::Instance()->IsType(ee::CameraMgr::ORTHO)) {
		ee::OrthoCamera* cam = static_cast<ee::OrthoCamera*>(ee::CameraMgr::Instance()->GetCamera());
		cam->Translate(sm::vec2(dx, dy));
	}
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

}
}