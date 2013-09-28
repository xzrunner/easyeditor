#pragma once

#include "common/Object.h"

namespace d2d
{
	class EditPanel;

	class AbstractEditOP : public Object
	{
	public:
		AbstractEditOP(EditPanel* editPanel) {
			m_editPanel = editPanel;
		}
		virtual ~AbstractEditOP() {}

		virtual bool onKeyDown(int keyCode) { return false; }
		virtual bool onKeyUp(int keyCode) { return false; }
		virtual bool onMouseLeftDown(int x, int y) { return false; }
		virtual bool onMouseLeftUp(int x, int y) { return false; }
		virtual bool onMouseRightDown(int x, int y) { return false; }
		virtual bool onMouseRightUp(int x, int y) { return false; }
		virtual bool onMouseMove(int x, int y) { return false; }
		virtual bool onMouseDrag(int x, int y) { return false; }
		virtual bool onMouseLeftDClick(int x, int y) { return false; }
		virtual bool onMouseWheelRotation(int x, int y, int direction) { return false; }

		virtual bool onPopMenuSelected(int type) { return false; }

		virtual bool onActive() { return false; }

		virtual bool onDraw() const { return false; }
		virtual bool clear() { return false; }

	protected:
		EditPanel* m_editPanel;

	}; // AbstractEditOP
}

