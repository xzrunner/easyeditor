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
	
	virtual bool OnKeyDown(int keyCode);
	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseRightDown(int x, int y);
	virtual bool OnMouseMove(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

	virtual bool OnDraw() const;
	virtual bool Clear();

private:
	d2d::MultiShapesImpl* m_shapes_impl;

	d2d::OneFloatValue* m_node_capture;

	NodeAddr m_captured;

	d2d::Vector m_pos;

}; // EditPointOP

}

#endif // _LIBSHAPE_EDIT_POINT_OP_H_