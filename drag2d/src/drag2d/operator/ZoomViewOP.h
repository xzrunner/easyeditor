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

		virtual bool OnKeyDown(int keyCode);
		virtual bool OnKeyUp(int keyCode);
		virtual bool OnMouseLeftDown(int x, int y);
		virtual bool OnMouseLeftUp(int x, int y);
		virtual bool OnMouseRightDown(int x, int y);
		virtual bool OnMouseRightUp(int x, int y);
		virtual bool OnMouseMove(int x, int y);
		virtual bool OnMouseDrag(int x, int y);
		virtual bool OnMouseWheelRotation(int x, int y, int direction);

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

