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
	ShapesPanelImpl();
	ShapesPanelImpl(IDataContainer* container);
	virtual ~ShapesPanelImpl();

	//
	// MultiShapesImpl interface
	//
	virtual void TraverseShapes(IVisitor& visitor, 
		DataTraverseType type = DT_ALL) const;

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	void InitSubjects();

private:
	IDataContainer* m_container;

}; // ShapesPanelImpl

}

#endif // _DRAG2D_SHAPES_PANEL_IMPL_H_