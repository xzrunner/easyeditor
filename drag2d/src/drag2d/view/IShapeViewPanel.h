#ifndef _DRAG2D_ISHAPE_VIEW_PANEL_H_
#define _DRAG2D_ISHAPE_VIEW_PANEL_H_

namespace d2d
{

class IShape;
class ShapeSelection;

class IShapeViewPanel
{
public:
	virtual ~IShapeViewPanel() {}
	virtual void SelectShape(IShape* shape) = 0;
	virtual void SelectMultiShapes(ShapeSelection* selection) = 0;
	virtual void RemoveShape(IShape* shape) = 0;
}; // IShapeViewPanel

}

#endif