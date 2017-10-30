#include "ShapesPanelImpl.h"
#include "DataContainer.h"
#include "ShapesContainer.h"
#include "StageCanvas.h"
#include "Shape.h"
#include "subject_id.h"
#include "shape_msg.h"
#include "panel_msg.h"

namespace ee
{

ShapesPanelImpl::ShapesPanelImpl()
	: MultiShapesImpl()
{
	m_container = std::make_shared<ShapesContainer>();

	InitSubjects();
}

ShapesPanelImpl::ShapesPanelImpl(const std::shared_ptr<DataContainer<Shape>>& container)
	: MultiShapesImpl()
	, m_container(container)
{
	InitSubjects();
}

void ShapesPanelImpl::TraverseShapes(RefVisitor<ee::Shape>& visitor, DataTraverseType type/* = e_allExisting*/) const
{
	m_container->Traverse(visitor, true);
}

void ShapesPanelImpl::OnNotify(int sj_id, void* ud)
{
	MultiShapesImpl::OnNotify(sj_id, ud);

	switch (sj_id)
	{
	case MSG_REMOVE_SHAPE:
		if (m_container->Remove(*(ee::ShapePtr*)ud)) {
			SetCanvasDirtySJ::Instance()->SetDirty();
		}
		break;
	case MSG_INSERT_SHAPE:
		if (m_container->Insert(*(ee::ShapePtr*)ud)) {
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

}