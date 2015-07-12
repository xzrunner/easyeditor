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
	ShapesPanelImpl(EditPanel* stage);
	ShapesPanelImpl(EditPanel* stage, IDataContainer* container);
	virtual ~ShapesPanelImpl();

	//
	// MultiShapesImpl interface
	//
	virtual bool InsertShape(IShape* shape);
	virtual bool RemoveShape(IShape* shape);
	virtual bool ClearAllShapes();
	virtual void TraverseShapes(IVisitor& visitor, 
		DataTraverseType type = DT_ALL) const;

private:
	EditPanel* m_stage;

	IDataContainer* m_container;

}; // ShapesPanelImpl

}

#endif // _DRAG2D_SHAPES_PANEL_IMPL_H_