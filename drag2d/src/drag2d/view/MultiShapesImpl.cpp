#include "MultiShapesImpl.h"
#include "ShapeSelection.h"

#include "message/subject_id.h"
#include "message/SelectShapeSJ.h"
#include "message/RemoveShapeSJ.h"

namespace d2d
{

MultiShapesImpl::MultiShapesImpl(EditPanelImpl* stage)
{
	m_shape_selection = new ShapeSelection(stage);

	m_subjects.push_back(SelectShapeSJ::Instance());
// 	m_subjects.push_back(SelectShapeSetSJ::Instance());
// 	m_subjects.push_back(RemoveShapeSJ::Instance());
	for (int i = 0; i < m_subjects.size(); ++i) {
		m_subjects[i]->Register(this);
	}
}

MultiShapesImpl::~MultiShapesImpl()
{
	m_shape_selection->Release();

	for (int i = 0; i < m_subjects.size(); ++i) {
		m_subjects[i]->UnRegister(this);
	}
}

void MultiShapesImpl::Notify(int sj_id, void* ud)
{
	switch (sj_id)
	{
	case MSG_SELECT_SHAPE:
		m_shape_selection->Clear();
		m_shape_selection->Add((IShape*)ud);
		break;
// 	case SELECT_SHAPE_SET:
// 		break;
// 	case REMOVE_SHAPE:
// 		break;
	}
}

IShape* MultiShapesImpl::QueryShapeByPos(const Vector& pos) const
{
	IShape* result = NULL;
	TraverseShapes(PointQueryVisitor(pos, &result), DT_EDITABLE);
	return result;
}

void MultiShapesImpl::QueryShapesByRect(const Rect& rect, std::vector<IShape*>& result) const
{
	TraverseShapes(RectQueryVisitor(rect, result), DT_EDITABLE);
}

void MultiShapesImpl::ClearSelectedShape()
{
	if (m_shape_selection->IsEmpty()) {
		return;
	}

	std::vector<IShape*> shapes;
	m_shape_selection->Traverse(FetchAllVisitor<IShape>(shapes));
	for (int i = 0, n = shapes.size(); i < n; ++i) {
		RemoveShapeSJ::Instance()->Remove(shapes[i]);
	}

	m_shape_selection->Clear();
}

//////////////////////////////////////////////////////////////////////////
// class MultiShapesImpl::PointQueryVisitor
//////////////////////////////////////////////////////////////////////////

MultiShapesImpl::PointQueryVisitor::PointQueryVisitor(const Vector& pos, IShape** pResult)
	: m_pos(pos)
{
	m_pResult = pResult;
	*m_pResult = NULL;
}

void MultiShapesImpl::PointQueryVisitor::Visit(Object* object, bool& bFetchNext)
{
	IShape* shape = static_cast<IShape*>(object);
	if (shape->isContain(m_pos))
	{
		*m_pResult = shape;
		bFetchNext = false;
	}
	else
	{
		bFetchNext = true;
	}
}

//////////////////////////////////////////////////////////////////////////
// class MultiShapesImpl::RectQueryVisitor
//////////////////////////////////////////////////////////////////////////

MultiShapesImpl::RectQueryVisitor::RectQueryVisitor(const Rect& rect, std::vector<IShape*>& result)
	: m_rect(rect), m_result(result)
{
}

void MultiShapesImpl::RectQueryVisitor::Visit(Object* object, bool& bFetchNext)
{
	IShape* shape = static_cast<IShape*>(object);
	if (shape->isIntersect(m_rect))
		m_result.push_back(shape);
	bFetchNext = true;
}

} // d2d