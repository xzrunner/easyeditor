#include "EditCameraOP.h"
#include "StageCanvas.h"
#include "TranslateCameraState.h"
#include "RotateCameraState.h"

namespace libsketch
{

EditCameraOP::EditCameraOP(d2d::EditPanel* stage)
	: d2d::AbstractEditOP(stage)
	, m_curr(NULL)
{
	e3d::StageCanvas* canvas 
		= static_cast<e3d::StageCanvas*>(stage->getCanvas());

	m_translate = new TranslateCameraState(canvas);
	m_rotate = new RotateCameraState(canvas);
	m_zoom = NULL;
}

bool EditCameraOP::onMouseLeftDown(int x, int y)
{
	if (d2d::AbstractEditOP::onMouseLeftDown(x, y)) { return true; }
	m_curr = m_translate;
	m_curr->OnMousePress(ivec2(x, y));
	return false;

}

bool EditCameraOP::onMouseLeftUp(int x, int y)
{
	if (d2d::AbstractEditOP::onMouseLeftUp(x, y)) { return true; }
	m_curr->OnMouseRelease(ivec2(x, y));
	return false;
}

bool EditCameraOP::onMouseRightDown(int x, int y)
{
	if (d2d::AbstractEditOP::onMouseRightDown(x, y)) { return true; }
	m_curr = m_rotate;
	m_curr->OnMousePress(ivec2(x, y));
	return false;
}

bool EditCameraOP::onMouseRightUp(int x, int y)
{
	if (d2d::AbstractEditOP::onMouseRightUp(x, y)) { return true; }
	m_curr->OnMouseRelease(ivec2(x, y));
	return false;
}

bool EditCameraOP::onMouseDrag(int x, int y)
{
	if (d2d::AbstractEditOP::onMouseDrag(x, y)) { return true; }
	m_curr->OnMouseMove(ivec2(x, y));
	return false;
}

}