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
	ShapesPanelImpl(EditPanelImpl* stage);
	ShapesPanelImpl(EditPanelImpl* stage, IDataContainer* container);
	virtual ~ShapesPanelImpl();

	//
	//	interface IShapeViewPanel
	//
	virtual void RemoveShape(IShape* shape);

	//
	// MultiShapesImpl interface
	//
	virtual bool InsertShape(IShape* shape);
	virtual bool ClearAllShapes();
	virtual void TraverseShapes(IVisitor& visitor, 
		DataTraverseType type = DT_ALL) const;

private:
	EditPanelImpl* m_stage;

	IDataContainer* m_container;

}; // ShapesPanelImpl

}

#endif // _DRAG2D_SHAPES_PANEL_IMPL_H_