#include "ShapesPanelImpl.h"
#include "IDataContainer.h"
#include "ShapesContainer.h"
#include "IStageCanvas.h"

#include "common/tools.h"
#include "dataset/IShape.h"

#include "message/subject_id.h"
#include "message/RemoveShapeSJ.h"
#include "message/InsertShapeSJ.h"
#include "message/ClearShapeSJ.h"
#include "message/SetCanvasDirtySJ.h"

namespace d2d
{

ShapesPanelImpl::ShapesPanelImpl()
	: MultiShapesImpl()
{
	m_container = new ShapesContainer();

	InitSubjects();
}

ShapesPanelImpl::ShapesPanelImpl(IDataContainer* container)
	: MultiShapesImpl()
{
	m_container = container;
	m_container->Retain();

	InitSubjects();
}

ShapesPanelImpl::~ShapesPanelImpl()
{
	m_container->Release();
}

void ShapesPanelImpl::TraverseShapes(IVisitor& visitor, DataTraverseType type/* = e_allExisting*/) const
{
	m_container->Traverse(visitor, true);
}

void ShapesPanelImpl::OnNotify(int sj_id, void* ud)
{
	MultiShapesImpl::OnNotify(sj_id, ud);

	switch (sj_id)
	{
	case MSG_REMOVE_SHAPE:
		if (m_container->Remove((IShape*)ud)) {
			SetCanvasDirtySJ::Instance()->SetDirty();
		}
		break;
	case MSG_INSERT_SHAPE:
		if (m_container->Insert((IShape*)ud)) {
			SetCanvasDirtySJ::Instance()->SetDirty();
		}
		break;
	case MSG_CLEAR_SHAPE:
		if (m_container->Clear()) {
			SetCanvasDirtySJ::Instance()->SetDirty();
		}
		break;
	}
}

void ShapesPanelImpl::InitSubjects()
{
	RegistSubject(RemoveShapeSJ::Instance());
	RegistSubject(InsertShapeSJ::Instance());
	RegistSubject(ClearShapeSJ::Instance());
}

} // d2d