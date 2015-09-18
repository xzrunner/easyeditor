#include "EditQuadOP.h"
#include "StagePanel.h"
#include "QuadIcon.h"

namespace eicon
{

const float EditQuadOP::CTRL_NODE_RADIUS = 5.0f;

EditQuadOP::EditQuadOP(StagePanel* stage)
	: d2d::ZoomViewOP(stage, stage->GetStageImpl(), true)
{
}

bool EditQuadOP::OnMouseLeftDown(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseLeftDown(x, y)) {
		return true;
	}
	
	

	return false;
}

bool EditQuadOP::OnMouseLeftUp(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseLeftUp(x, y)) {
		return true;
	}

	return false;
}

bool EditQuadOP::OnMouseDrag(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseDrag(x, y)) {
		return true;
	}

	return false;
}

bool EditQuadOP::OnActive()
{
	if (d2d::ZoomViewOP::OnActive()) {
		return true;
	}

	QuadIcon* icon = new QuadIcon;
	static_cast<StagePanel*>(m_wnd)->SetIcon(icon);
	icon->Release();	

	return false;
}

bool EditQuadOP::OnDraw() const
{
	if (d2d::ZoomViewOP::OnDraw()) {
		return true;
	}

	Icon* icon = static_cast<StagePanel*>(m_wnd)->GetIcon();
	if (!icon) {
		return false;
	}

	const d2d::Image* img = icon->GetImage();
	if (!img) {
		return false;
	}

	float w = img->GetClippedWidth(),
		h = img->GetClippedHeight();
	d2d::PrimitiveDraw::rect(d2d::Vector(0, 0), w * 0.5f, h * 0.5f, 
		d2d::LIGHT_RED_THIN_LINE);

	

	return false;
}

}