#ifndef _EASYSHAPE_EDIT_POLYLINE_OP_CPP_
#define _EASYSHAPE_EDIT_POLYLINE_OP_CPP_

#include "EditPolylineOP.h"
#include "EditPolylineImpl.h"

template <typename TBase, typename TSelected>
eshape::EditPolylineOP<TBase, TSelected>::
EditPolylineOP(wxWindow* wnd, ee::EditPanelImpl* stage, 
			   ee::MultiShapesImpl* shapes_impl,
			   ee::PropertySettingPanel* property,
			   ee::OneFloatValue* node_capture,
			   ee::EditCMPT* cmpt)
	: TBase(wnd, stage, shapes_impl)
	, m_node_capture(node_capture)
{
	m_select_op = std::make_shared<TSelected>(wnd, stage, shapes_impl, cmpt);
	m_impl = new EditPolylineImpl(wnd, stage, shapes_impl, property, m_node_capture, *this, *m_select_op, this);
}

template <typename TBase, typename TSelected>
eshape::EditPolylineOP<TBase, TSelected>::
~EditPolylineOP() 
{
	delete m_impl;
}

template <typename TBase, typename TSelected>
bool eshape::EditPolylineOP<TBase, TSelected>::
OnKeyDown(int keyCode) 
{
	return m_impl->OnKeyDown(keyCode);
}

template <typename TBase, typename TSelected>
bool eshape::EditPolylineOP<TBase, TSelected>::
OnMouseLeftDown(int x, int y) 
{
	return m_impl->OnMouseLeftDown(x, y);
}

template <typename TBase, typename TSelected>
bool eshape::EditPolylineOP<TBase, TSelected>::
OnMouseLeftUp(int x, int y) 
{
	if (TBase::OnMouseLeftUp(x, y)) return true;
	return m_impl->OnMouseLeftUp(x, y);
}

template <typename TBase, typename TSelected>
bool eshape::EditPolylineOP<TBase, TSelected>::
OnMouseRightDown(int x, int y) 
{
	return m_impl->OnMouseRightDown(x, y);
}

template <typename TBase, typename TSelected>
bool eshape::EditPolylineOP<TBase, TSelected>::
OnMouseMove(int x, int y) 
{
	if (TBase::OnMouseMove(x, y)) return true;
	return m_impl->OnMouseMove(x, y);
}

template <typename TBase, typename TSelected>
bool eshape::EditPolylineOP<TBase, TSelected>::
OnMouseDrag(int x, int y) 
{
	if (TBase::OnMouseDrag(x, y)) return true;
	return m_impl->OnMouseDrag(x, y);
}

template <typename TBase, typename TSelected>
bool eshape::EditPolylineOP<TBase, TSelected>::
OnDraw() const 
{
	if (TBase::OnDraw()) return true;
	m_impl->OnDraw();
	return false;
}

template <typename TBase, typename TSelected>
bool eshape::EditPolylineOP<TBase, TSelected>::
Clear() 
{
	if (TBase::Clear()) return true;
	m_impl->Clear();
	return false;
}

template <typename TBase, typename TSelected>
bool eshape::EditPolylineOP<TBase, TSelected>::
OnMouseLeftDownBase(int x, int y)
{
	return TBase::OnMouseLeftDown(x, y);
}

template <typename TBase, typename TSelected>
bool eshape::EditPolylineOP<TBase, TSelected>::
OnMouseRightDownBase(int x, int y)
{
	return TBase::OnMouseRightDown(x, y);
}

template <typename TBase, typename TSelected>
bool eshape::EditPolylineOP<TBase, TSelected>::
IsDirty() const
{
	return m_impl->IsDirty();
}

template <typename TBase, typename TSelected>
void eshape::EditPolylineOP<TBase, TSelected>::
ResetDirty()
{
	return m_impl->ResetDirty();
}

#endif // _EASYSHAPE_EDIT_POLYLINE_OP_CPP_