#include "RotateCameraOP.h"
#include "StageCanvas.h"

namespace libsketch
{

RotateCameraOP::RotateCameraOP(d2d::EditPanel* stage)
	: d2d::AbstractEditOP(stage)
	, m_impl(static_cast<e3d::StageCanvas*>(m_editPanel->getCanvas()))
{
}

bool RotateCameraOP::onMouseRightDown(int x, int y)
{
	if (d2d::AbstractEditOP::onMouseRightDown(x, y)) { return true; }
	m_impl.OnMousePress(ivec2(x, y));
	return false;
}

bool RotateCameraOP::onMouseRightUp(int x, int y)
{
	if (d2d::AbstractEditOP::onMouseRightUp(x, y)) { return true; }
	m_impl.OnMouseRelease(ivec2(x, y));
	return false;
}

bool RotateCameraOP::onMouseDrag(int x, int y)
{
	if (d2d::AbstractEditOP::onMouseDrag(x, y)) { return true; }
	m_impl.OnMouseMove(ivec2(x, y));
	return false;
}

}