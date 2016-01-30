#include "PreviewOP.h"
#include "PreviewSettings.h"

#include <easyanim.h>

namespace eanim
{

PreviewOP::PreviewOP(wxWindow* wnd, d2d::EditPanelImpl* stage, 
					 PlaySettings& settings, d2d::PlayControl& control)
	: d2d::ZoomViewOP(wnd, stage, true, true, false)
	, m_settings(settings)
	, m_control(control)
{
}

bool PreviewOP::OnKeyDown(int keyCode)
{
	if (d2d::ZoomViewOP::OnKeyDown(keyCode))
		return true;

	if (keyCode == WXK_SPACE)
	{
		m_settings.isStop = !m_settings.isStop;
		m_control.Reset();
	}

	return false;
}

}