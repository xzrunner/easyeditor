#ifndef _LIBSHAPE_EDIT_POLYLINE_OP_CPP_
#define _LIBSHAPE_EDIT_POLYLINE_OP_CPP_

#include "EditPolylineOP.h"
#include "EditPolylineImpl.h"
#include "NodeCaptureStatic.h"

template <typename TBase, typename TSelected>
libshape::EditPolylineOP<TBase, TSelected>::
EditPolylineOP(d2d::EditPanel* editPanel, d2d::MultiShapesImpl* shapesImpl,
			   d2d::PropertySettingPanel* propertyPanel,
			   NodeCaptureCMPT<EditPolylineOP>* cmpt,
			   float node_capture_scope)
	: TBase(editPanel, shapesImpl)
	, m_node_capture(NULL)
{
	m_select_op = new TSelected(editPanel, shapesImpl, propertyPanel, cmpt);
	INodeCapture* node_capture = cmpt;
	if (!node_capture) {
		node_capture = m_node_capture = new NodeCaptureStatic(node_capture_scope);
	}
	m_impl = new EditPolylineImpl(editPanel, shapesImpl, propertyPanel, node_capture, this, m_select_op, this);
}

template <typename TBase, typename TSelected>
libshape::EditPolylineOP<TBase, TSelected>::
~EditPolylineOP() 
{
	delete m_node_capture;
	delete m_select_op;
}

template <typename TBase, typename TSelected>
bool libshape::EditPolylineOP<TBase, TSelected>::
onKeyDown(int keyCode) 
{
	return m_impl->OnKeyDown(keyCode);
}

template <typename TBase, typename TSelected>
bool libshape::EditPolylineOP<TBase, TSelected>::
onMouseLeftDown(int x, int y) 
{
	return m_impl->OnMouseLeftDown(x, y);
}

template <typename TBase, typename TSelected>
bool libshape::EditPolylineOP<TBase, TSelected>::
onMouseLeftUp(int x, int y) 
{
	if (TBase::onMouseLeftUp(x, y)) return true;
	return m_impl->OnMouseLeftUp(x, y);
}

template <typename TBase, typename TSelected>
bool libshape::EditPolylineOP<TBase, TSelected>::
onMouseRightDown(int x, int y) 
{
	return m_impl->OnMouseRightDown(x, y);
}

template <typename TBase, typename TSelected>
bool libshape::EditPolylineOP<TBase, TSelected>::
onMouseMove(int x, int y) 
{
	if (TBase::onMouseMove(x, y)) return true;
	return m_impl->OnMouseMove(x, y);
}

template <typename TBase, typename TSelected>
bool libshape::EditPolylineOP<TBase, TSelected>::
onMouseDrag(int x, int y) 
{
	if (TBase::onMouseDrag(x, y)) return true;
	return m_impl->OnMouseDrag(x, y);
}

template <typename TBase, typename TSelected>
bool libshape::EditPolylineOP<TBase, TSelected>::
onDraw() const 
{
	if (TBase::onDraw()) return true;
	m_impl->OnDraw();
	return false;
}

template <typename TBase, typename TSelected>
bool libshape::EditPolylineOP<TBase, TSelected>::
clear() 
{
	if (TBase::clear()) return true;
	m_impl->Clear();
	return false;
}

template <typename TBase, typename TSelected>
bool libshape::EditPolylineOP<TBase, TSelected>::
OnMouseLeftDownBase(int x, int y)
{
	return TBase::onMouseLeftDown(x, y);
}

template <typename TBase, typename TSelected>
bool libshape::EditPolylineOP<TBase, TSelected>::
OnMouseRightDownBase(int x, int y)
{
	return TBase::onMouseRightDown(x, y);
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