#ifndef _LIBSHAPE_DRAW_RECTANGLE_OP_H_
#define _LIBSHAPE_DRAW_RECTANGLE_OP_H_

#include <drag2d.h>

namespace libshape
{

class DrawRectangleOP : public d2d::ZoomViewOP
{
public:
	DrawRectangleOP(d2d::EditPanel* editPanel, bool bOpenRightTap = true);
	
	virtual bool onMouseLeftDown(int x, int y);
	virtual bool onMouseLeftUp(int x, int y);
	virtual bool onMouseDrag(int x, int y);

	virtual bool onDraw(const d2d::Screen& scr) const;
	virtual bool clear();

protected:
	d2d::Vector m_firstPos, m_currPos;

	d2d::ShapeStyle m_style;

}; // DrawRectangleOP

}

#endif // _LIBSHAPE_DRAW_RECTANGLE_OP_H_