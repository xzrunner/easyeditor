#ifndef _EASYEDITOR_SHAPES_PANEL_IMPL_H_
#define _EASYEDITOR_SHAPES_PANEL_IMPL_H_

#include "MultiShapesImpl.h"
#include "DataContainer.h"

namespace ee
{

class Shape;

class ShapesPanelImpl : public MultiShapesImpl
{
public:
	ShapesPanelImpl();
	ShapesPanelImpl(const std::shared_ptr<DataContainer<Shape>>& container);

	//
	// MultiShapesImpl interface
	//
	virtual void TraverseShapes(RefVisitor<ee::Shape>& visitor, 
		DataTraverseType type = DT_ALL) const override;

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud) override;

private:
	void InitSubjects();

private:
	std::shared_ptr<DataContainer<Shape>> m_container;

}; // ShapesPanelImpl

}

#endif // _EASYEDITOR_SHAPES_PANEL_IMPL_H_