#ifndef _EASYSHAPE_EDIT_POINT_OP_H_
#define _EASYSHAPE_EDIT_POINT_OP_H_

#include "NodeCapture.h"

#include <ee/ZoomViewOP.h>

namespace ee { class OneFloatValue; }

namespace eshape
{

class EditPointOP : public ee::ZoomViewOP
{
public:
	EditPointOP(wxWindow* wnd, ee::EditPanelImpl* stage, 
		ee::MultiShapesImpl* shapes_impl, ee::OneFloatValue* node_capture);
	
	virtual bool OnKeyDown(int keyCode) override;
	virtual bool OnMouseLeftDown(int x, int y) override;
	virtual bool OnMouseLeftUp(int x, int y) override;
	virtual bool OnMouseRightDown(int x, int y) override;
	virtual bool OnMouseMove(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;

	virtual bool OnDraw() const override;
	virtual bool Clear() override;

private:
	ee::MultiShapesImpl* m_shapes_impl;

	ee::OneFloatValue* m_node_capture;

	NodeAddr m_captured;

	sm::vec2 m_pos;

}; // EditPointOP

}

#endif // _EASYSHAPE_EDIT_POINT_OP_H_