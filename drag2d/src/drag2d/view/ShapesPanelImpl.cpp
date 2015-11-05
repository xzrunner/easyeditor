#include "ShapesPanelImpl.h"
#include "IDataContainer.h"
#include "ShapesContainer.h"
#include "IStageCanvas.h"
#include "EditPanelImpl.h"

#include "common/tools.h"
#include "dataset/IShape.h"

#include "message/subject_id.h"
#include "message/RemoveShapeSJ.h"

namespace d2d
{

ShapesPanelImpl::ShapesPanelImpl(EditPanelImpl* stage)
	: MultiShapesImpl(stage)
	, m_stage(stage)
{
	m_container = new ShapesContainer();

	RemoveShapeSJ::Instance()->Register(this);
}

ShapesPanelImpl::ShapesPanelImpl(EditPanelImpl* stage, IDataContainer* container)
	: MultiShapesImpl(stage)
	, m_stage(stage)
{
	m_container = container;
	m_container->Retain();

	RemoveShapeSJ::Instance()->Register(this);
}

ShapesPanelImpl::~ShapesPanelImpl()
{
	m_container->Release();

	RemoveShapeSJ::Instance()->UnRegister(this);
}

void ShapesPanelImpl::Notify(int sj_id, void* ud)
{
	MultiShapesImpl::Notify(sj_id, ud);
	if (sj_id == MSG_REMOVE_SHAPE) {
		bool dirty = m_container->Remove((IShape*)ud);
		if (dirty) {
			m_stage->SetCanvasDirty();
		}
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