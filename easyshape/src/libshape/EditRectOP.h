#ifndef _LIBSHAPE_EDIT_RECT_OP_H_
#define _LIBSHAPE_EDIT_RECT_OP_H_

#include <drag2d.h>

#include "NodeCapture.h"

namespace libshape
{

class INodeCapture;

class EditRectOP : public d2d::ZoomViewOP
{
public:
	EditRectOP(d2d::EditPanel* editPanel, d2d::MultiShapesImpl* shapesImpl,
		d2d::PropertySettingPanel* propertyPanel, INodeCapture* node_capture);

	virtual bool onKeyDown(int keyCode);
	virtual bool onMouseLeftDown(int x, int y);
	virtual bool onMouseLeftUp(int x, int y);
	virtual bool onMouseRightDown(int x, int y);
	virtual bool onMouseMove(int x, int y);
	virtual bool onMouseDrag(int x, int y);

	virtual bool onDraw() const;
	virtual bool clear();

private:
	d2d::MultiShapesImpl* m_shapesImpl;

	d2d::PropertySettingPanel* m_propertyPanel;

	INodeCapture* m_node_capture;

	d2d::Vector m_firstPress;
	d2d::Vector m_currPos;

	NodeAddr m_captured;

	d2d::ShapeStyle m_style;

}; // EditRectOP

}

#endif // _LIBSHAPE_EDIT_RECT_OP_H_