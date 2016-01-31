#ifndef _LIBSHAPE_EDIT_POINT_OP_H_
#define _LIBSHAPE_EDIT_POINT_OP_H_



#include "NodeCapture.h"

namespace libshape
{

class EditPointOP : public ee::ZoomViewOP
{
public:
	EditPointOP(wxWindow* wnd, ee::EditPanelImpl* stage, 
		ee::MultiShapesImpl* shapes_impl, ee::OneFloatValue* node_capture);
	
	virtual bool OnKeyDown(int keyCode);
	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseRightDown(int x, int y);
	virtual bool OnMouseMove(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

	virtual bool OnDraw() const;
	virtual bool Clear();

private:
	ee::MultiShapesImpl* m_shapes_impl;

	ee::OneFloatValue* m_node_capture;

	NodeAddr m_captured;

	ee::Vector m_pos;

}; // EditPointOP

}

#endif // _LIBSHAPE_EDIT_POINT_OP_H_