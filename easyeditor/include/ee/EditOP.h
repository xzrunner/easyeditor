#ifndef _EASYEDITOR_EDIT_OP_H_
#define _EASYEDITOR_EDIT_OP_H_

#include <CU_RefCountObj.h>

#include <wx/cursor.h>

namespace ee
{

class EditPanelImpl;

class EditOP : public cu::RefCountObj
{
public:
	EditOP(wxWindow* wnd, EditPanelImpl* stage);

	virtual bool OnKeyDown(int keyCode) { return false; }
	virtual bool OnKeyUp(int keyCode) { return false; }
	virtual bool OnChar(int keyCode) { return false; }

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

}; // EditOP

}

#endif // _EASYEDITOR_EDIT_OP_H_