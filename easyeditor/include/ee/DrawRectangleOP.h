#ifndef _EASYEDITOR_DRAW_RECTANGLE_OP_
#define _EASYEDITOR_DRAW_RECTANGLE_OP_

#include "ZoomViewOP.h"

#include <SM_Vector.h>

namespace ee
{

class DrawRectangleOP : public ZoomViewOP
{
public:
	DrawRectangleOP(wxWindow* wnd, EditPanelImpl* stage, bool bOpenRightTap = true);
	
	virtual bool OnMouseLeftDown(int x, int y) override;
	virtual bool OnMouseLeftUp(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;

	virtual bool OnDraw() const override;
	virtual bool Clear() override;

protected:
	sm::vec2 m_first_pos, m_last_pos;

}; // DrawRectangleOP

}

#endif // _EASYEDITOR_DRAW_RECTANGLE_OP_