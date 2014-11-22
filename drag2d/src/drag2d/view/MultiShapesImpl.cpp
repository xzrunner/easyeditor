#include "MultiShapesImpl.h"

namespace d2d
{

MultiShapesImpl::MultiShapesImpl(wxWindow* wnd)
{
	m_wnd = wnd;
	m_shapeSelection = new ShapeSelection;
}

MultiShapesImpl::~MultiShapesImpl()
{
	m_shapeSelection->Release();
}

IShape* MultiShapesImpl::queryShapeByPos(const Vector& pos) const
{
	IShape* result = NULL;
	traverseShapes(PointQueryVisitor(pos, &result), e_editable);
	return result;
}

void MultiShapesImpl::queryShapesByRect(const Rect& rect, std::vector<IShape*>& result) const
{
	traverseShapes(RectQueryVisitor(rect, result), e_editable);
}

void MultiShapesImpl::removeShapeSelection()
{
	if (!m_shapeSelection->IsEmpty())
	{
		m_shapeSelection->Traverse(RemoveSelectionVisitor(this));
		m_shapeSelection->Clear();
		refresh();
	}
}

void MultiShapesImpl::refresh()
{
	m_wnd->Refresh();
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

void MultiShapesImpl::PointQueryVisitor::visit(Object* object, bool& bFetchNext)
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

void MultiShapesImpl::RectQueryVisitor::visit(Object* object, bool& bFetchNext)
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

void MultiShapesImpl::RemoveSelectionVisitor::visit(Object* object, bool& bFetchNext)
{
	m_shapesImpl->removeShape(static_cast<IShape*>(object));
	bFetchNext = true;
}

} // d2d