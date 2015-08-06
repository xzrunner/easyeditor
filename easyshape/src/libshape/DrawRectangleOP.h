#ifndef _LIBSHAPE_DRAW_RECTANGLE_OP_H_
#define _LIBSHAPE_DRAW_RECTANGLE_OP_H_

#include <drag2d.h>

namespace libshape
{

class DrawRectangleOP : public d2d::ZoomViewOP
{
public:
	DrawRectangleOP(wxWindow* wnd, d2d::EditPanelImpl* stage, bool bOpenRightTap = true);
	
	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

	virtual bool OnDraw() const;
	virtual bool Clear();

protected:
	d2d::Vector m_firstPos, m_currPos;

	d2d::ShapeStyle m_style;

}; // DrawRectangleOP

}

#endif // _LIBSHAPE_DRAW_RECTANGLE_OP_H_