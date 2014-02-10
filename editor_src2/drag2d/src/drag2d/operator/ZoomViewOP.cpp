#include "ZoomViewOP.h"

#include "view/Camera.h"
#include "common/Settings.h"

namespace d2d
{

ZoomViewOP::ZoomViewOP(EditPanel* editPanel, bool bMouseMoveFocus) 
	: AbstractEditOP(editPanel)
	, m_openRightTap(true)
	, m_bMouseMoveFocus(bMouseMoveFocus)
	, m_onRightBtnPan(false)
{
	m_lastPos.setInvalid();
}

bool ZoomViewOP::onKeyDown(int keyCode)
{
	if (!Settings::bZoomOnlyUseMouseWheel && keyCode == WXK_SPACE)
		m_editPanel->SetCursor(wxCURSOR_HAND);
	return false;
}

bool ZoomViewOP::onKeyUp(int keyCode)
{
	if (!Settings::bZoomOnlyUseMouseWheel && keyCode == WXK_SPACE)
		m_editPanel->SetCursor(wxCURSOR_ARROW);
	return false;
}

bool ZoomViewOP::onMouseLeftDown(int x, int y)
{
	if (!Settings::bZoomOnlyUseMouseWheel)
		m_lastPos.setInvalid();

	if (!Settings::bZoomOnlyUseMouseWheel && wxGetKeyState(WXK_SPACE))
	{
		m_lastPos.set(x, y);
		return true;
	}
	else
	{
		return false;
	}
}

bool ZoomViewOP::onMouseLeftUp(int x, int y)
{
	if (!Settings::bZoomOnlyUseMouseWheel && wxGetKeyState(WXK_SPACE))
	{
		m_lastPos.setInvalid();
		return true;
	}
	else
	{
		return false;
	}
}

bool ZoomViewOP::onMouseRightDown(int x, int y) 
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

bool ZoomViewOP::onMouseRightUp(int x, int y) 
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

bool ZoomViewOP::onMouseMove(int x, int y) 
{
	if (m_bMouseMoveFocus) 
		m_editPanel->SetFocus();

	return false;
}

bool ZoomViewOP::onMouseDrag(int x, int y)
{
	if (Settings::bZoomOnlyUseMouseWheel) return false;

	if (wxGetKeyState(WXK_SPACE) || m_onRightBtnPan)
	{
		if (!m_lastPos.isValid())
			m_lastPos.set(x, y);
		else
		{
			Vector currPos(x, y);

			Vector cameraPos = m_editPanel->getCamera()->getCenter();
			Vector offset = m_lastPos - currPos;
			offset *= m_editPanel->getCamera()->getScale();
			offset.y = -offset.y;
			m_editPanel->getCamera()->setCenter(cameraPos + offset);

			m_lastPos = currPos;

			m_editPanel->Refresh();
		}
		return true;
	}
	else
	{
		m_lastPos.setInvalid();
		return false;
	}
}

bool ZoomViewOP::onMouseWheelRotation(int x, int y, int direction) 
{
//	if (Settings::bZoomOnlyUseMouseWheel)
		m_editPanel->onMouseWheelRotation(x, y, direction);
// 	else
// 		m_editPanel->onMouseWheelRotation(m_editPanel->GetSize().GetWidth() * 0.5f, m_editPanel->GetSize().GetHeight() * 0.5f, direction);

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