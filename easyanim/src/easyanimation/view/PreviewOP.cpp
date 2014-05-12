#include "PreviewOP.h"
#include "PreviewSettings.h"
#include "PreviewControl.h"

namespace eanim
{

PreviewOP::PreviewOP(d2d::EditPanel* editPanel, PlaySettings& settings,
					 PreviewControl& control)
	: d2d::ZoomViewOP(editPanel, true, true, false)
	, m_settings(settings)
	, m_control(control)
{
}

bool PreviewOP::onKeyDown(int keyCode)
{
	if (d2d::ZoomViewOP::onKeyDown(keyCode))
		return true;

	if (keyCode == WXK_SPACE)
	{
		m_settings.isStop = !m_settings.isStop;
		m_control.reset();
	}

	return false;
}

}