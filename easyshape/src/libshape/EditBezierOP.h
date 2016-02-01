#ifndef _EASYSHAPE_EDIT_BEZIER_OP_H_
#define _EASYSHAPE_EDIT_BEZIER_OP_H_

#include "NodeCapture.h"

#include <ee/ZoomViewOP.h>

namespace ee { class OneFloatValue; class PropertySettingPanel; }

namespace eshape
{

class EditBezierOP : public ee::ZoomViewOP
{
public:
	EditBezierOP(wxWindow* wnd, ee::EditPanelImpl* stage, ee::MultiShapesImpl* shapes_impl,
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

	ee::Vector m_firstPress;
	ee::Vector m_curr_pos;

	NodeAddr m_captured;

}; // EditBezierOP

}

#endif // _EASYSHAPE_EDIT_BEZIER_OP_H_