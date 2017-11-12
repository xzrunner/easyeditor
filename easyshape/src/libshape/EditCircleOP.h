#ifndef _EASYSHAPE_EDIT_CIRCLE_OP_H_
#define _EASYSHAPE_EDIT_CIRCLE_OP_H_

#include "NodeCapture.h"

#include <ee/ZoomViewOP.h>

namespace ee { class OneFloatValue; class PropertySettingPanel; }

namespace eshape
{

class EditCircleOP : public ee::ZoomViewOP
{
public:
	EditCircleOP(wxWindow* wnd, ee::EditPanelImpl* stage, ee::MultiShapesImpl* shapes_impl,
		ee::PropertySettingPanel* property, ee::OneFloatValue* node_capture);

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

	ee::PropertySettingPanel* m_property;

	ee::OneFloatValue* m_node_capture;

	sm::vec2 m_first_pos, m_curr_pos;

	NodeAddr m_captured;

}; // EditCircleOP

}

#endif // _EASYSHAPE_EDIT_CIRCLE_OP_H_