#include "ZoomViewOP.h"

#include "view/Camera.h"
#include "view/IStageCanvas.h"
#include "common/config.h"
#include "common/SettingData.h"

namespace d2d
{

ZoomViewOP::ZoomViewOP(EditPanel* editPanel, bool bMouseMoveFocus,
					   bool bOpenRightTap, bool bOpenLeftTap) 
	: AbstractEditOP(editPanel)
	, m_bMouseMoveFocus(bMouseMoveFocus)
	, m_openRightTap(bOpenRightTap)
	, m_openLeftTap(bOpenLeftTap)
	, m_onRightBtnPan(false)
{
	const SettingData& setting = Config::Instance()->GetSettings();
	m_bMouseMoveFocus = setting.auto_get_focus;

	m_lastPos.setInvalid();
}

bool ZoomViewOP::OnKeyDown(int keyCode)
{
	if (m_openLeftTap && keyCode == WXK_SPACE) {
		m_stage->SetCursor(wxCURSOR_HAND);
	} else if (keyCode == WXK_ESCAPE) {
		Camera* cam = m_stage->GetCamera();
		cam->Reset();
	}

	return false;
}

bool ZoomViewOP::OnKeyUp(int keyCode)
{
	if (m_openLeftTap && keyCode == WXK_SPACE)
		m_stage->SetCursor(wxCURSOR_ARROW);
	return false;
}

bool ZoomViewOP::OnMouseLeftDown(int x, int y)
{
	if (m_openLeftTap)
		m_lastPos.setInvalid();

	if (m_openLeftTap && wxGetKeyState(WXK_SPACE))
	{
		m_lastPos.set(x, y);
		return true;
	}
	else
	{
		return false;
	}
}

bool ZoomViewOP::OnMouseLeftUp(int x, int y)
{
	if (m_openLeftTap && wxGetKeyState(WXK_SPACE))
	{
		m_lastPos.setInvalid();
		return true;
	}
	else
	{
		return false;
	}
}

bool ZoomViewOP::OnMouseRightDown(int x, int y) 
{ 
	if (m_openRightTap)
	{
		m_onRightBtnPan = true;
		m_lastPos.set(x, y);
		return true;
	}
	else
	{
		return false;
	}
}

bool ZoomViewOP::OnMouseRightUp(int x, int y) 
{ 
	if (m_openRightTap)
	{
		m_onRightBtnPan = false;
		m_lastPos.setInvalid();
		return true;
	}
	else
	{
		return false;
	}
}

bool ZoomViewOP::OnMouseMove(int x, int y) 
{
	if (m_bMouseMoveFocus) 
		m_stage->SetFocus();

	return false;
}

bool ZoomViewOP::OnMouseDrag(int x, int y)
{
	if (!m_openLeftTap) return false;

	if (wxGetKeyState(WXK_SPACE) || m_onRightBtnPan)
	{
		if (!m_lastPos.isValid())
			m_lastPos.set(x, y);
		else
		{
			Vector currPos(x, y);

			Vector offset = m_lastPos - currPos;
			offset.y = -offset.y;
			m_stage->GetCamera()->Translate(offset);

			m_lastPos = currPos;

			m_stage->SetCanvasDirty();
		}
		return true;
	}
	else
	{
		m_lastPos.setInvalid();
		return false;
	}
}

bool ZoomViewOP::OnMouseWheelRotation(int x, int y, int direction) 
{
//	if (Settings::bZoomOnlyUseMouseWheel)
		m_stage->OnMouseWheelRotation(x, y, direction);
// 	else
// 		m_stage->OnMouseWheelRotation(m_stage->GetSize().GetWidth() * 0.5f, m_stage->GetSize().GetHeight() * 0.5f, direction);

		m_stage->SetCanvasDirty();

	return false;
}

void ZoomViewOP::enableRightTap(bool enable)
{
	if (m_openRightTap == enable)
		return;

	m_openRightTap = enable;
	if (enable)
	{
		m_onRightBtnPan = true;
	}
	else
	{
		m_onRightBtnPan = false;
		m_lastPos.setInvalid();
	}
}

} // d2d