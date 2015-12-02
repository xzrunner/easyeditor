#ifndef _DRAG2D_ABSTRACT_EDITOP_H_
#define _DRAG2D_ABSTRACT_EDITOP_H_

#include "common/Object.h"

#include <wx/cursor.h>

namespace d2d
{

class EditPanelImpl;

class AbstractEditOP : public Object
{
public:
	AbstractEditOP(wxWindow* wnd, EditPanelImpl* stage);

	virtual bool OnKeyDown(int keyCode) { return false; }
	virtual bool OnKeyUp(int keyCode) { return false; }
	virtual bool OnMouseLeftDown(int x, int y) { return false; }
	virtual bool OnMouseLeftUp(int x, int y) { return false; }
	virtual bool OnMouseRightDown(int x, int y) { return false; }
	virtual bool OnMouseRightUp(int x, int y) { return false; }
	virtual bool OnMouseMove(int x, int y) { return false; }
	virtual bool OnMouseDrag(int x, int y) { return false; }
	virtual bool OnMouseLeftDClick(int x, int y) { return false; }
	virtual bool OnMouseWheelRotation(int x, int y, int direction) { return false; }

	virtual bool OnPopMenuSelected(int type) { return false; }

	virtual bool OnActive();

	virtual bool OnDraw() const { return false; }
	virtual bool Clear() { return false; }

	virtual bool IsEmpty() const { return true; }

protected:
	wxWindow* m_wnd;

	EditPanelImpl* m_stage;

	wxCursor m_cursor;

}; // AbstractEditOP

}

#endif // _DRAG2D_ABSTRACT_EDITOP_H_