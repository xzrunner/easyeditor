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
	m_canvas = static_cast<e3d::StageCanvas*>(stage->GetCanvas());

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

bool EditCameraOP::OnKeyDown(int keyCode)
{
	if (d2d::AbstractEditOP::OnKeyDown(keyCode)) { return true; }
	
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

bool EditCameraOP::OnMouseLeftDown(int x, int y)
{
	if (d2d::AbstractEditOP::OnMouseLeftDown(x, y)) { return true; }
	m_curr = m_translate;
	m_curr->OnMousePress(ivec2(x, y));
	return false;
}

bool EditCameraOP::OnMouseLeftUp(int x, int y)
{
	if (d2d::AbstractEditOP::OnMouseLeftUp(x, y)) { return true; }
	m_curr->OnMouseRelease(ivec2(x, y));
	return false;
}

bool EditCameraOP::OnMouseRightDown(int x, int y)
{
	if (d2d::AbstractEditOP::OnMouseRightDown(x, y)) { return true; }
	m_curr = m_rotate;
	m_curr->OnMousePress(ivec2(x, y));
	return false;
}

bool EditCameraOP::OnMouseRightUp(int x, int y)
{
	if (d2d::AbstractEditOP::OnMouseRightUp(x, y)) { return true; }
	m_curr->OnMouseRelease(ivec2(x, y));
	return false;
}

bool EditCameraOP::OnMouseMove(int x, int y)
{
	m_stage->SetFocus();
	return false;
}

bool EditCameraOP::OnMouseDrag(int x, int y)
{
	if (d2d::AbstractEditOP::OnMouseDrag(x, y)) { return true; }
	m_curr->OnMouseMove(ivec2(x, y));
	return false;
}

bool EditCameraOP::OnMouseWheelRotation(int x, int y, int direction)
{
	if (d2d::AbstractEditOP::OnMouseWheelRotation(x, y, direction)) { return true; }
	m_zoom->OnMouseWheelRotation(ivec2(x, y), direction);
	return false;}

}