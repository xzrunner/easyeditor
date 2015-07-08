#include "MultiShapesImpl.h"

namespace d2d
{

MultiShapesImpl::MultiShapesImpl(wxWindow* wnd)
{
	m_wnd = wnd;
	m_shape_selection = new ShapeSelection;
}

MultiShapesImpl::~MultiShapesImpl()
{
	m_shape_selection->Release();
}

void MultiShapesImpl::SelectShape(IShape* shape)
{
	m_shape_selection->Clear();
	m_shape_selection->Add(shape);
}

void MultiShapesImpl::SelectMultiShapes(ShapeSelection* selection)
{
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

bool MultiShapesImpl::ClearShapeSelection()
{
	if (!m_shape_selection->IsEmpty()) {
		m_shape_selection->Traverse(RemoveSelectionVisitor(this));
		m_shape_selection->Clear();
		return true;
	} else {
		return false;
	}
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

//////////////////////////////////////////////////////////////////////////
// class MultiShapesImpl::RemoveSelectionVisitor
//////////////////////////////////////////////////////////////////////////

MultiShapesImpl::RemoveSelectionVisitor::RemoveSelectionVisitor(MultiShapesImpl* shapesImpl)
{
	m_shapesImpl = shapesImpl;
}

void MultiShapesImpl::RemoveSelectionVisitor::Visit(Object* object, bool& bFetchNext)
{
	m_shapesImpl->RemoveShape(static_cast<IShape*>(object));
	bFetchNext = true;
}

} // d2d