#ifndef _DRAG2D_ZOOM_VIWE_OP_H_
#define _DRAG2D_ZOOM_VIWE_OP_H_

#include "AbstractEditOP.h"

#include "view/EditPanel.h"

namespace d2d
{

class IEditOPState;

class ZoomViewOP : public AbstractEditOP
{
public:
	ZoomViewOP(EditPanel* stage, bool mouse_move_focus,
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
	void SetRightPan(bool enable) { 
		m_open_right_pan = enable;
	}

private:
	void SwitchState(IEditOPState* state);

private:
	bool m_mouse_move_focus;

	bool m_open_right_pan;
	bool m_open_left_pan;

	IEditOPState* m_op_state;

	IEditOPState* m_view_state;
	IEditOPState* m_left_pan_state;
	IEditOPState* m_right_pan_state;

}; // ZoomViewOP

}

#endif // _DRAG2D_ZOOM_VIWE_OP_H_