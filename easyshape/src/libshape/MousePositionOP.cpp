#include "MousePositionOP.h"

namespace libshape
{

MousePositionOP::MousePositionOP(wxWindow* wnd, ee::EditPanelImpl* stage, 
								 ee::MousePositionCMPT* cmpt)
	: ee::MousePositionOP(wnd, stage, cmpt)
{
	m_draw_impl = new DrawPolylineOP(wnd, stage, false);
}

MousePositionOP::~MousePositionOP()
{
	delete m_draw_impl;
}

bool MousePositionOP::OnMouseLeftDown(int x, int y)
{
	return m_draw_impl->OnMouseLeftDown(x, y);
}

bool MousePositionOP::OnMouseRightDown(int x, int y)
{
	return m_draw_impl->OnMouseRightDown(x, y);
}

bool MousePositionOP::OnMouseMove(int x, int y)
{
	if (ee::MousePositionOP::OnMouseMove(x, y)) return true;
	return m_draw_impl->OnMouseMove(x, y);
}

bool MousePositionOP::OnMouseLeftDClick(int x, int y)
{
	return m_draw_impl->OnMouseLeftDClick(x, y);
}

bool MousePositionOP::OnDraw() const
{
	return m_draw_impl->OnDraw();
}

bool MousePositionOP::Clear()
{
	return m_draw_impl->Clear();
}

}