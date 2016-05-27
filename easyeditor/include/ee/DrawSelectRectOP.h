#ifndef _EASYEDITOR_DRAW_SELECT_RECT_OP_
#define _EASYEDITOR_DRAW_SELECT_RECT_OP_

#include "ZoomViewOP.h"

#include <SM_Vector.h>

namespace ee
{

class DrawSelectRectOP : public ZoomViewOP
{
public:
	DrawSelectRectOP(wxWindow* wnd, EditPanelImpl* stage, 
		bool bOpenRightTap = true);
	
	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

	virtual bool OnDraw() const;
	virtual bool Clear();

protected:
	sm::vec2 m_first_pos, m_last_pos;

}; // DrawSelectRectOP

}

#endif // _EASYEDITOR_DRAW_SELECT_RECT_OP_