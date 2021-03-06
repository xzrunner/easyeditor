#ifndef _EASYSHAPE_DRAW_RECTANGLE_OP_H_
#define _EASYSHAPE_DRAW_RECTANGLE_OP_H_

#include <ee/ZoomViewOP.h>

#include <SM_Vector.h>

namespace eshape
{

class DrawRectangleOP : public ee::ZoomViewOP
{
public:
	DrawRectangleOP(wxWindow* wnd, ee::EditPanelImpl* stage, bool bOpenRightTap = true);
	
	virtual bool OnMouseLeftDown(int x, int y) override;
	virtual bool OnMouseLeftUp(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;

	virtual bool OnDraw() const override;
	virtual bool Clear() override;

protected:
	sm::vec2 m_first_pos, m_curr_pos;

}; // DrawRectangleOP

}

#endif // _EASYSHAPE_DRAW_RECTANGLE_OP_H_