#include "PreviewOP.h"
#include "PreviewSettings.h"

namespace eanim
{

PreviewOP::PreviewOP(d2d::EditPanel* editPanel, PlaySettings& settings)
	: d2d::ZoomViewOP(editPanel, true, true, false)
	, m_settings(settings)
{
}

bool PreviewOP::onKeyDown(int keyCode)
{
	if (d2d::ZoomViewOP::onKeyDown(keyCode))
		return true;

	if (keyCode == WXK_SPACE)
	{
		m_settings.isStop = !m_settings.isStop;
	}

	return false;
}

}