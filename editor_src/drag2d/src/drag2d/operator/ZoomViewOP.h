#pragma once

#include "AbstractEditOP.h"

#include "view/EditPanel.h"

namespace d2d
{
	class ZoomViewOP : public AbstractEditOP
	{
	public:
		ZoomViewOP(EditPanel* editPanel, bool bMouseMoveFocus);

		virtual bool onKeyDown(int keyCode);
		virtual bool onKeyUp(int keyCode);
		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseLeftUp(int x, int y);
		virtual bool onMouseMove(int x, int y);
		virtual bool onMouseDrag(int x, int y);
		virtual bool onMouseWheelRotation(int x, int y, int direction);

	private:
		bool m_bMouseMoveFocus;

		Vector m_lastPos;

	}; // ZoomViewOP
}

