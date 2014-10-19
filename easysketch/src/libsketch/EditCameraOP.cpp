#include "EditCameraOP.h"
#include "StageCanvas.h"
#include "TranslateCameraState.h"
#include "RotateCameraState.h"
#include "ZoomCameraState.h"

namespace libsketch
{

EditCameraOP::EditCameraOP(d2d::EditPanel* stage)
	: d2d::AbstractEditOP(stage)
	, m_curr(NULL)
{
	m_canvas = static_cast<e3d::StageCanvas*>(stage->getCanvas());

	m_translate = new TranslateCameraState(m_canvas);
	m_rotate = new RotateCameraState(m_canvas);
	m_zoom = new ZoomCameraState(m_canvas);
}

EditCameraOP::~EditCameraOP()
{
	delete m_translate;
	delete m_rotate;
	delete m_zoom;
}

bool EditCameraOP::onKeyDown(int keyCode)
{
	if (d2d::AbstractEditOP::onKeyDown(keyCode)) { return true; }
	
	switch (keyCode)
	{
	case WXK_SPACE:
		{
			m_canvas->GetCamera3().Reset();
			m_canvas->Refresh();
		}
		break;
	}

	return false;
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

bool EditCameraOP::onMouseMove(int x, int y)
{
	m_editPanel->SetFocus();
	return false;
}

bool EditCameraOP::onMouseDrag(int x, int y)
{
	if (d2d::AbstractEditOP::onMouseDrag(x, y)) { return true; }
	m_curr->OnMouseMove(ivec2(x, y));
	return false;
}

bool EditCameraOP::onMouseWheelRotation(int x, int y, int direction)
{
	if (d2d::AbstractEditOP::onMouseWheelRotation(x, y, direction)) { return true; }
	m_zoom->OnMouseWheelRotation(ivec2(x, y), direction);
	return false;}

}