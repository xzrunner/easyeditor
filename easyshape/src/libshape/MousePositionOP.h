#ifndef _LIBSHAPE_MOUSE_POSITION_OP_H_
#define _LIBSHAPE_MOUSE_POSITION_OP_H_

#include <drag2d.h>

#include "DrawPolylineOP.h"

namespace libshape
{

class MousePositionOP : public d2d::MousePositionOP 
{
public:
	MousePositionOP(d2d::EditPanel* editPanel, d2d::MousePositionCMPT* cmpt);
	virtual ~MousePositionOP();

	virtual bool onMouseLeftDown(int x, int y);
	virtual bool onMouseRightDown(int x, int y);
	virtual bool onMouseMove(int x, int y);
	virtual bool onMouseLeftDClick(int x, int y);

	virtual bool onDraw() const;
	virtual bool clear();

private:
	DrawPolylineOP* m_draw_impl;

}; // MousePositionOP

}

#endif // _LIBSHAPE_MOUSE_POSITION_OP_H_