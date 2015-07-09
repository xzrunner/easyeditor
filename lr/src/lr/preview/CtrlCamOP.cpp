#include "CtrlCamOP.h"

#include "frame/SettingCfg.h"

namespace lr
{
namespace preview
{

CtrlCamOP::CtrlCamOP(d2d::EditPanel* editPanel)
	: d2d::AbstractEditOP(editPanel)
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
	m_stage->GetCamera()->Translate(d2d::Vector(dx, dy));
	m_stage->GetCanvas()->SetDirty();
}

}
}