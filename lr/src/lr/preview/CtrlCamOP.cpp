#include "CtrlCamOP.h"

#include "frame/SettingCfg.h"

#include <ee/EditPanelImpl.h>
#include <ee/Camera.h>
#include <ee/panel_msg.h>
#include <ee/OrthoCamera.h>

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
	static_cast<ee::OrthoCamera*>(m_stage->GetCamera())->Translate(ee::Vector(dx, dy));
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

}
}