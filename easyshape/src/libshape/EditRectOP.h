#ifndef _EASYSHAPE_EDIT_RECT_OP_H_
#define _EASYSHAPE_EDIT_RECT_OP_H_

#include "NodeCapture.h"

#include <ee/ZoomViewOP.h>
#include <ee/ShapeStyle.h>

namespace ee { class OneFloatValue; class PropertySettingPanel; }

namespace eshape
{

class ee::OneFloatValue;

class EditRectOP : public ee::ZoomViewOP
{
public:
	EditRectOP(wxWindow* wnd, ee::EditPanelImpl* stage, ee::MultiShapesImpl* shapes_impl,
		ee::PropertySettingPanel* property, ee::OneFloatValue* node_capture);

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

	ee::PropertySettingPanel* m_property;

	ee::OneFloatValue* m_node_capture;

	ee::Vector m_first_press;
	ee::Vector m_curr_pos;

	NodeAddr m_captured;

	ee::ShapeStyle m_style;

}; // EditRectOP

}

#endif // _EASYSHAPE_EDIT_RECT_OP_H_