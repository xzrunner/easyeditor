#include "MultiShapesImpl.h"
#include "ShapeSelection.h"
#include "subject_id.h"
#include "shape_msg.h"
#include "FetchAllVisitor.h"

namespace ee
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
	m_shape_selection->RemoveReference();
}

ShapePtr MultiShapesImpl::QueryShapeByPos(const sm::vec2& pos) const
{
	ShapePtr result;
	TraverseShapes(PointQueryVisitor(pos, result), DT_EDITABLE);
	return result;
}

void MultiShapesImpl::QueryShapesByRect(const sm::rect& rect, std::vector<ShapePtr>& result) const
{
	TraverseShapes(RectQueryVisitor(rect, result), DT_EDITABLE);
}

void MultiShapesImpl::ClearSelectedShape()
{
	if (m_shape_selection->IsEmpty()) {
		return;
	}

	std::vector<ShapePtr> shapes;
	m_shape_selection->Traverse(FetchAllRefVisitor<Shape>(shapes));
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
		m_shape_selection->Add(*(ShapePtr*)ud);
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

MultiShapesImpl::PointQueryVisitor::PointQueryVisitor(const sm::vec2& pos, ShapePtr& ret)
	: m_pos(pos)
	, m_ret(ret)
{
}

void MultiShapesImpl::PointQueryVisitor::Visit(const ShapePtr& shape, bool& next)
{
	if (shape && shape->IsContain(m_pos))
	{
		m_ret = shape;
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

MultiShapesImpl::RectQueryVisitor::RectQueryVisitor(const sm::rect& rect, std::vector<ShapePtr>& result)
	: m_rect(rect)
	, m_result(result)
{
}

void MultiShapesImpl::RectQueryVisitor::Visit(const ShapePtr& shape, bool& next)
{
	if (shape && shape->IsIntersect(m_rect))
		m_result.push_back(shape);
	next = true;
}

}