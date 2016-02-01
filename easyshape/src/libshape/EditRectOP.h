#ifndef _EASYSHAPE_EDIT_RECT_OP_H_
#define _EASYSHAPE_EDIT_RECT_OP_H_



#include "NodeCapture.h"

namespace eshape
{

class ee::OneFloatValue;

class EditRectOP : public ee::ZoomViewOP
{
public:
	EditRectOP(wxWindow* wnd, ee::EditPanelImpl* stage, ee::MultiShapesImpl* shapesImpl,
		ee::PropertySettingPanel* propertyPanel, ee::OneFloatValue* node_capture);

	virtual bool OnKeyDown(int keyCode);
	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseRightDown(int x, int y);
	virtual bool OnMouseMove(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

	virtual bool OnDraw() const;
	virtual bool Clear();

private:
	ee::MultiShapesImpl* m_shapesImpl;

	ee::PropertySettingPanel* m_propertyPanel;

	ee::OneFloatValue* m_node_capture;

	ee::Vector m_firstPress;
	ee::Vector m_currPos;

	NodeAddr m_captured;

	ee::ShapeStyle m_style;

}; // EditRectOP

}

#endif // _EASYSHAPE_EDIT_RECT_OP_H_