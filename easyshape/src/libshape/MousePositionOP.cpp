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

bool MousePositionOP::onMouseLeftDown(int x, int y)
{
	return m_draw_impl->onMouseLeftDown(x, y);
}

bool MousePositionOP::onMouseRightDown(int x, int y)
{
	return m_draw_impl->onMouseRightDown(x, y);
}

bool MousePositionOP::onMouseMove(int x, int y)
{
	if (d2d::MousePositionOP::onMouseMove(x, y)) return true;
	return m_draw_impl->onMouseMove(x, y);
}

bool MousePositionOP::onMouseLeftDClick(int x, int y)
{
	return m_draw_impl->onMouseLeftDClick(x, y);
}

bool MousePositionOP::onDraw() const
{
	return m_draw_impl->onDraw();
}

bool MousePositionOP::clear()
{
	return m_draw_impl->clear();
}

}