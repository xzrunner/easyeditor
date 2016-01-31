#ifndef _EASYEDITOR_ZOOM_VIWE_OP_H_
#define _EASYEDITOR_ZOOM_VIWE_OP_H_

#include "EditOP.h"

namespace ee
{

class EditOPState;

class ZoomViewOP : public EditOP
{
public:
	ZoomViewOP(wxWindow* wnd, EditPanelImpl* stage, bool mouse_move_focus,
		bool right_tap = false, bool left_tap = true);
	virtual ~ZoomViewOP();

	virtual bool OnKeyDown(int keyCode);
	virtual bool OnKeyUp(int keyCode);
	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseRightDown(int x, int y);
	virtual bool OnMouseRightUp(int x, int y);
	virtual bool OnMouseMove(int x, int y);
	virtual bool OnMouseDrag(int x, int y);
	virtual bool OnMouseWheelRotation(int x, int y, int direction);

	void SetMouseMoveFocus(bool enable) {
		m_mouse_move_focus = enable;
	}

protected:
	void SetRightPan(bool enable);

private:
	void SwitchState(EditOPState* state);

private:
	bool m_mouse_move_focus;

	bool m_open_right_pan;
	bool m_open_left_pan;

	EditOPState* m_op_state;

	EditOPState* m_view_state;
	EditOPState* m_left_pan_state;
	EditOPState* m_right_pan_state;

}; // ZoomViewOP

}

#endif // _EASYEDITOR_ZOOM_VIWE_OP_H_