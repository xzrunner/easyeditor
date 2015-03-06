#ifndef _DRAG2D_SHAPES_PANEL_IMPL_H_
#define _DRAG2D_SHAPES_PANEL_IMPL_H_

#include "MultiShapesImpl.h"

namespace d2d
{

class IShape;
class IDataContainer;

class ShapesPanelImpl : public MultiShapesImpl
{
public:
	ShapesPanelImpl(wxWindow* parent);
	ShapesPanelImpl(wxWindow* parent, IDataContainer* container);
	virtual ~ShapesPanelImpl();

	//
	// MultiShapesImpl interface
	//
	virtual void traverseShapes(IVisitor& visitor, 
		TraverseType type = e_allExisting) const;
	virtual void removeShape(IShape* shape);
	virtual void insertShape(IShape* shape);
	virtual void clearShapes();

private:
	IDataContainer* m_container;

}; // ShapesPanelImpl

}

#endif // _DRAG2D_SHAPES_PANEL_IMPL_H_