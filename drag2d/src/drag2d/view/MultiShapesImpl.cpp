#include "MultiShapesImpl.h"
#include "ShapeSelection.h"

#include "message/subject_id.h"
#include "message/SelectShapeSJ.h"
#include "message/RemoveShapeSJ.h"

namespace d2d
{

MultiShapesImpl::MultiShapesImpl()
{
	m_shape_selection = new ShapeSelection();

	RegistSubject(SelectShapeSJ::Instance());
// 	RegistSubject(SelectShapeSetSJ::Instance());
// 	RegistSubject(RemoveShapeSJ::Instance());
}

MultiShapesImpl::~MultiShapesImpl()
{
	m_shape_selection->Release();
}

Shape* MultiShapesImpl::QueryShapeByPos(const Vector& pos) const
{
	Shape* result = NULL;
	TraverseShapes(PointQueryVisitor(pos, &result), DT_EDITABLE);
	return result;
}

void MultiShapesImpl::QueryShapesByRect(const Rect& rect, std::vector<Shape*>& result) const
{
	TraverseShapes(RectQueryVisitor(rect, result), DT_EDITABLE);
}

void MultiShapesImpl::ClearSelectedShape()
{
	if (m_shape_selection->IsEmpty()) {
		return;
	}

	std::vector<Shape*> shapes;
	m_shape_selection->Traverse(FetchAllVisitor<Shape>(shapes));
	for (int i = 0, n = shapes.size(); i < n; ++i) {
		RemoveShapeSJ::Instance()->Remove(shapes[i]);
	}

	m_shape_selection->Clear();
}

void MultiShapesImpl::OnNotify(int sj_id, void* ud)
{
	switch (sj_id)
	{
	case MSG_SELECT_SHAPE:
		m_shape_selection->Clear();
		m_shape_selection->Add((Shape*)ud);
		break;
// 	case SELECT_SHAPE_SET:
// 		break;
// 	case REMOVE_SHAPE:
// 		break;
	}
}

//////////////////////////////////////////////////////////////////////////
// class MultiShapesImpl::PointQueryVisitor
//////////////////////////////////////////////////////////////////////////

MultiShapesImpl::PointQueryVisitor::PointQueryVisitor(const Vector& pos, Shape** pResult)
	: m_pos(pos)
{
	m_pResult = pResult;
	*m_pResult = NULL;
}

void MultiShapesImpl::PointQueryVisitor::Visit(Object* object, bool& next)
{
	Shape* shape = static_cast<Shape*>(object);
	if (shape->IsContain(m_pos))
	{
		*m_pResult = shape;
		next = false;
	}
	else
	{
		next = true;
	}
}

//////////////////////////////////////////////////////////////////////////
// class MultiShapesImpl::RectQueryVisitor
//////////////////////////////////////////////////////////////////////////

MultiShapesImpl::RectQueryVisitor::RectQueryVisitor(const Rect& rect, std::vector<Shape*>& result)
	: m_rect(rect), m_result(result)
{
}

void MultiShapesImpl::RectQueryVisitor::Visit(Object* object, bool& next)
{
	Shape* shape = static_cast<Shape*>(object);
	if (shape->IsIntersect(m_rect))
		m_result.push_back(shape);
	next = true;
}

}