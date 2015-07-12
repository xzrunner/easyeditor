#include "ShapesPanelImpl.h"
#include "IDataContainer.h"
#include "ShapesContainer.h"
#include "IStageCanvas.h"
#include "EditPanel.h"

#include "common/tools.h"
#include "dataset/IShape.h"

namespace d2d
{

ShapesPanelImpl::ShapesPanelImpl(EditPanel* stage)
	: MultiShapesImpl(stage)
	, m_stage(stage)
{
	m_container = new ShapesContainer();
}

ShapesPanelImpl::ShapesPanelImpl(EditPanel* stage, IDataContainer* container)
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

bool ShapesPanelImpl::InsertShape(IShape* shape)
{
	bool ret = m_container->Insert(shape);
	if (ret) {
		m_stage->SetCanvasDirty();
	}
	return ret;
}

bool ShapesPanelImpl::RemoveShape(IShape* shape)
{
	bool ret = m_container->Remove(shape);
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