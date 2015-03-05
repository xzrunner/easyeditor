#pragma once

#include "AbstractEditOP.h"

#include "view/EditPanel.h"

namespace d2d
{
	class ZoomViewOP : public AbstractEditOP
	{
	public:
		ZoomViewOP(EditPanel* editPanel, bool bMouseMoveFocus,
			bool bOpenRightTap = false, bool bOpenLeftTap = true);

		virtual bool onKeyDown(int keyCode);
		virtual bool onKeyUp(int keyCode);
		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseLeftUp(int x, int y);
		virtual bool onMouseRightDown(int x, int y);
		virtual bool onMouseRightUp(int x, int y);
		virtual bool onMouseMove(int x, int y);
		virtual bool onMouseDrag(int x, int y);
		virtual bool onMouseWheelRotation(int x, int y, int direction);

		void setMouseMoveFocus(bool enable) {
			m_bMouseMoveFocus = enable;
		}

	protected:
		void enableRightTap(bool enable);

	private:
		bool m_bMouseMoveFocus;
		bool m_onRightBtnPan;

		Vector m_lastPos;

		bool m_openRightTap;
		bool m_openLeftTap;

	}; // ZoomViewOP
}

