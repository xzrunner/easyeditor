#ifndef _LIBSHAPE_EDIT_POLYLINE_OP_CPP_
#define _LIBSHAPE_EDIT_POLYLINE_OP_CPP_

#include "EditPolylineOP.h"
#include "EditPolylineImpl.h"

template <typename TBase, typename TSelected>
libshape::EditPolylineOP<TBase, TSelected>::
EditPolylineOP(d2d::EditPanel* editPanel, d2d::MultiShapesImpl* shapesImpl,
			   d2d::PropertySettingPanel* propertyPanel,
			   d2d::ViewPanelMgr* view_panel_mgr,
			   d2d::OneFloatValue* node_capture,
			   d2d::AbstractEditCMPT* cmpt)
	: TBase(editPanel, shapesImpl)
	, m_node_capture(node_capture)
{
	m_select_op = new TSelected(editPanel, shapesImpl, view_panel_mgr, cmpt);
	m_impl = new EditPolylineImpl(editPanel, shapesImpl, propertyPanel, view_panel_mgr, m_node_capture, this, m_select_op, this);
}

template <typename TBase, typename TSelected>
libshape::EditPolylineOP<TBase, TSelected>::
~EditPolylineOP() 
{
	delete m_impl;
	delete m_select_op;
}

template <typename TBase, typename TSelected>
bool libshape::EditPolylineOP<TBase, TSelected>::
OnKeyDown(int keyCode) 
{
	return m_impl->OnKeyDown(keyCode);
}

template <typename TBase, typename TSelected>
bool libshape::EditPolylineOP<TBase, TSelected>::
OnMouseLeftDown(int x, int y) 
{
	return m_impl->OnMouseLeftDown(x, y);
}

template <typename TBase, typename TSelected>
bool libshape::EditPolylineOP<TBase, TSelected>::
OnMouseLeftUp(int x, int y) 
{
	if (TBase::OnMouseLeftUp(x, y)) return true;
	return m_impl->OnMouseLeftUp(x, y);
}

template <typename TBase, typename TSelected>
bool libshape::EditPolylineOP<TBase, TSelected>::
OnMouseRightDown(int x, int y) 
{
	return m_impl->OnMouseRightDown(x, y);
}

template <typename TBase, typename TSelected>
bool libshape::EditPolylineOP<TBase, TSelected>::
OnMouseMove(int x, int y) 
{
	if (TBase::OnMouseMove(x, y)) return true;
	return m_impl->OnMouseMove(x, y);
}

template <typename TBase, typename TSelected>
bool libshape::EditPolylineOP<TBase, TSelected>::
OnMouseDrag(int x, int y) 
{
	if (TBase::OnMouseDrag(x, y)) return true;
	return m_impl->OnMouseDrag(x, y);
}

template <typename TBase, typename TSelected>
bool libshape::EditPolylineOP<TBase, TSelected>::
OnDraw() const 
{
	if (TBase::OnDraw()) return true;
	m_impl->OnDraw();
	return false;
}

template <typename TBase, typename TSelected>
bool libshape::EditPolylineOP<TBase, TSelected>::
clear() 
{
	if (TBase::Clear()) return true;
	m_impl->Clear();
	return false;
}

template <typename TBase, typename TSelected>
bool libshape::EditPolylineOP<TBase, TSelected>::
OnMouseLeftDownBase(int x, int y)
{
	return TBase::OnMouseLeftDown(x, y);
}

template <typename TBase, typename TSelected>
bool libshape::EditPolylineOP<TBase, TSelected>::
OnMouseRightDownBase(int x, int y)
{
	return TBase::OnMouseRightDown(x, y);
}

template <typename TBase, typename TSelected>
bool libshape::EditPolylineOP<TBase, TSelected>::
IsDirty() const
{
	return m_impl->IsDirty();
}

template <typename TBase, typename TSelected>
void libshape::EditPolylineOP<TBase, TSelected>::
ResetDirty()
{
	return m_impl->ResetDirty();
}

#endif // _LIBSHAPE_EDIT_POLYLINE_OP_CPP_