#include "MousePositionOP.h"

namespace libshape
{

MousePositionOP::MousePositionOP(d2d::EditPanel* editPanel, d2d::MousePositionCMPT* cmpt)
	: d2d::MousePositionOP(editPanel, cmpt)
{
	m_draw_impl = new DrawPolylineOP(editPanel, false);
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
	if (d2d::MousePositionOP::OnMouseMove(x, y)) return true;
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