#include "ee0/EditOP.h"

namespace ee0
{

bool EditOP::OnKeyDown(int keyCode)
{
	if (m_prev_op && m_prev_op->OnKeyDown(keyCode)) {
		return true;
	} else {
		return false;
	}
}

bool EditOP::OnKeyUp(int keyCode)
{
	if (m_prev_op && m_prev_op->OnKeyUp(keyCode)) {
		return true;
	} else {
		return false;
	}
}

bool EditOP::OnChar(int keyCode)
{
	if (m_prev_op && m_prev_op->OnChar(keyCode)) {
		return true;
	} else {
		return false;
	}
}

bool EditOP::OnMouseLeftDown(int x, int y)
{
	if (m_prev_op && m_prev_op->OnMouseLeftDown(x, y)) {
		return true;
	} else {
		return false;
	}
}

bool EditOP::OnMouseLeftUp(int x, int y)
{
	if (m_prev_op && m_prev_op->OnMouseLeftUp(x, y)) {
		return true;
	} else {
		return false;
	}
}

bool EditOP::OnMouseRightDown(int x, int y)
{
	if (m_prev_op && m_prev_op->OnMouseRightDown(x, y)) {
		return true;
	} else {
		return false;
	}
}

bool EditOP::OnMouseRightUp(int x, int y)
{
	if (m_prev_op && m_prev_op->OnMouseRightUp(x, y)) {
		return true;
	} else {
		return false;
	}
}

bool EditOP::OnMouseMove(int x, int y)
{
	if (m_prev_op && m_prev_op->OnMouseMove(x, y)) {
		return true;
	} else {
		return false;
	}
}

bool EditOP::OnMouseDrag(int x, int y)
{
	if (m_prev_op && m_prev_op->OnMouseDrag(x, y)) {
		return true;
	} else {
		return false;
	}
}

bool EditOP::OnMouseLeftDClick(int x, int y)
{
	if (m_prev_op && m_prev_op->OnMouseLeftDClick(x, y)) {
		return true;
	} else {
		return false;
	}
}

bool EditOP::OnMouseWheelRotation(int x, int y, int direction)
{
	if (m_prev_op && m_prev_op->OnMouseWheelRotation(x, y, direction)) {
		return true;
	} else {
		return false;
	}
}

bool EditOP::OnPopMenuSelected(int type)
{
	if (m_prev_op && m_prev_op->OnPopMenuSelected(type)) {
		return true;
	} else {
		return false;
	}
}

bool EditOP::OnDraw() const
{
	if (m_prev_op && m_prev_op->OnDraw()) {
		return true;
	} else {
		return false;
	}
}

bool EditOP::Clear()
{
	if (m_prev_op && m_prev_op->Clear()) {
		return true;
	} else {
		return false;
	}
}

}