#ifndef _DRAG2D_ISHAPE_VIEW_PANEL_H_
#define _DRAG2D_ISHAPE_VIEW_PANEL_H_

#include "dataset/ObjSelectionSet.h"

namespace d2d
{

class IShapeViewPanel
{
public:
	virtual ~IShapeViewPanel() {}
	virtual void SelectShape(IShape* shape) = 0;
	virtual void SelectMultiShapes(ShapeSelection* selection) = 0;
}; // IShapeViewPanel

}

#endif