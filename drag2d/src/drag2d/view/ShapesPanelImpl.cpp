#include "ShapesPanelImpl.h"
#include "IDataContainer.h"
#include "ShapesContainer.h"
#include "IStageCanvas.h"
#include "EditPanelImpl.h"

#include "common/tools.h"
#include "dataset/IShape.h"

namespace d2d
{

ShapesPanelImpl::ShapesPanelImpl(EditPanelImpl* stage)
	: MultiShapesImpl(stage)
	, m_stage(stage)
{
	m_container = new ShapesContainer();
}

ShapesPanelImpl::ShapesPanelImpl(EditPanelImpl* stage, IDataContainer* container)
	: MultiShapesImpl(stage)
	, m_stage(stage)
{
	m_container = container;
	m_container->Retain();
}

ShapesPanelImpl::~ShapesPanelImpl()
{
	m_container->Release();
}

void ShapesPanelImpl::RemoveShape(IShape* shape)
{
	bool dirty = m_container->Remove(shape);
	if (dirty) {
		m_stage->SetCanvasDirty();
	}
}

bool ShapesPanelImpl::InsertShape(IShape* shape)
{
	bool ret = m_container->Insert(shape);
	if (ret) {
		m_stage->SetCanvasDirty();
	}
	return ret;
}

bool ShapesPanelImpl::ClearAllShapes()
{
	bool ret = m_container->Clear();
	if (ret) {
		m_stage->SetCanvasDirty();
	}
	return ret;
}

void ShapesPanelImpl::TraverseShapes(IVisitor& visitor, DataTraverseType type/* = e_allExisting*/) const
{
	m_container->Traverse(visitor, true);
}

} // d2d