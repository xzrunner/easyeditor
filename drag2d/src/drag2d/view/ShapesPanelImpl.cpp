#include "ShapesPanelImpl.h"
#include "IDataContainer.h"
#include "ShapesContainer.h"
#include "IStageCanvas.h"
#include "EditPanelImpl.h"

#include "common/tools.h"
#include "dataset/IShape.h"

#include "message/subject_id.h"
#include "message/RemoveShapeSJ.h"
#include "message/InsertShapeSJ.h"
#include "message/ClearShapeSJ.h"

namespace d2d
{

ShapesPanelImpl::ShapesPanelImpl(EditPanelImpl* stage)
	: MultiShapesImpl(stage)
	, m_stage(stage)
{
	m_container = new ShapesContainer();

	InitSubjects();
}

ShapesPanelImpl::ShapesPanelImpl(EditPanelImpl* stage, IDataContainer* container)
	: MultiShapesImpl(stage)
	, m_stage(stage)
{
	m_container = container;
	m_container->Retain();

	InitSubjects();
}

ShapesPanelImpl::~ShapesPanelImpl()
{
	m_container->Release();

	for (int i = 0, n = m_subjects.size(); i < n; ++i) {
		m_subjects[i]->UnRegister(this);
	}
}

void ShapesPanelImpl::Notify(int sj_id, void* ud)
{
	MultiShapesImpl::Notify(sj_id, ud);

	switch (sj_id)
	{
	case MSG_REMOVE_SHAPE:
		if (m_container->Remove((IShape*)ud)) {
			m_stage->SetCanvasDirty();
		}
		break;
	case MSG_INSERT_SHAPE:
		if (m_container->Insert((IShape*)ud)) {
			m_stage->SetCanvasDirty();
		}
		break;
	case MSG_CLEAR_SHAPE:
		if (m_container->Clear()) {
			m_stage->SetCanvasDirty();
		}
		break;
	}
}

void ShapesPanelImpl::TraverseShapes(IVisitor& visitor, DataTraverseType type/* = e_allExisting*/) const
{
	m_container->Traverse(visitor, true);
}

void ShapesPanelImpl::InitSubjects()
{
	m_subjects.push_back(RemoveShapeSJ::Instance());
	m_subjects.push_back(InsertShapeSJ::Instance());
	m_subjects.push_back(ClearShapeSJ::Instance());
	for (int i = 0, n = m_subjects.size(); i < n; ++i) {
		m_subjects[i]->Register(this);
	}
}

} // d2d