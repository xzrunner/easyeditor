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
	//	interface Observer
	//
	virtual void Notify(int sj_id, void* ud);

	//
	// MultiShapesImpl interface
	//
	virtual void TraverseShapes(IVisitor& visitor, 
		DataTraverseType type = DT_ALL) const;

private:
	void InitSubjects();

private:
	IDataContainer* m_container;

	std::vector<Subject*> m_subjects;

}; // ShapesPanelImpl

}

#endif // _DRAG2D_SHAPES_PANEL_IMPL_H_