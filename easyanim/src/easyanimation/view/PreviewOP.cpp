#include "PreviewOP.h"
#include "PreviewSettings.h"

#include <ee/PlayControl.h>

#include <easyanim.h>

namespace eanim
{

PreviewOP::PreviewOP(wxWindow* wnd, ee::EditPanelImpl* stage, 
					 PlaySettings& settings, ee::PlayControl& control)
	: ee::ZoomViewOP(wnd, stage, true, true, false)
	, m_settings(settings)
	, m_control(control)
{
}

bool PreviewOP::OnKeyDown(int keyCode)
{
	if (ee::ZoomViewOP::OnKeyDown(keyCode))
		return true;

	if (keyCode == WXK_SPACE)
	{
		m_settings.isStop = !m_settings.isStop;
		m_control.Reset();
	}

	return false;
}

}