#ifndef _LIBSHAPE_EDIT_BEZIER_OP_H_
#define _LIBSHAPE_EDIT_BEZIER_OP_H_

#include <drag2d.h>

#include "NodeCapture.h"

namespace libshape
{

class d2d::OneFloatValue;

class EditBezierOP : public d2d::ZoomViewOP
{
public:
	EditBezierOP(d2d::EditPanel* editPanel, d2d::MultiShapesImpl* shapesImpl,
		d2d::PropertySettingPanel* propertyPanel, d2d::OneFloatValue* node_capture);

	virtual bool OnKeyDown(int keyCode);
	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseRightDown(int x, int y);
	virtual bool OnMouseMove(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

	virtual bool OnDraw() const;
	virtual bool Clear();

private:
	d2d::MultiShapesImpl* m_shapesImpl;

	d2d::PropertySettingPanel* m_propertyPanel;

	d2d::OneFloatValue* m_node_capture;

	d2d::Vector m_firstPress;
	d2d::Vector m_currPos;

	NodeAddr m_captured;

}; // EditBezierOP

}

#endif // _LIBSHAPE_EDIT_BEZIER_OP_H_