#ifndef _LIBSHAPE_EDIT_POINT_OP_H_
#define _LIBSHAPE_EDIT_POINT_OP_H_

#include <drag2d.h>

#include "NodeCapture.h"

namespace libshape
{

class EditPointOP : public d2d::ZoomViewOP
{
public:
	EditPointOP(d2d::EditPanel* editpanel, d2d::MultiShapesImpl* shapes_impl,
		d2d::OneFloatValue* node_capture);
	
	virtual bool onKeyDown(int keyCode);
	virtual bool onMouseLeftDown(int x, int y);
	virtual bool onMouseLeftUp(int x, int y);
	virtual bool onMouseRightDown(int x, int y);
	virtual bool onMouseMove(int x, int y);
	virtual bool onMouseDrag(int x, int y);

	virtual bool onDraw() const;
	virtual bool clear();

private:
	d2d::MultiShapesImpl* m_shapes_impl;

	d2d::OneFloatValue* m_node_capture;

	NodeAddr m_captured;

	d2d::Vector m_pos;

}; // EditPointOP

}

#endif // _LIBSHAPE_EDIT_POINT_OP_H_