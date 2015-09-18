#include "EditQuadOP.h"
#include "StagePanel.h"
#include "QuadIcon.h"

namespace eicon
{

const float EditQuadOP::CTRL_NODE_RADIUS = 5.0f;

EditQuadOP::EditQuadOP(StagePanel* stage)
	: d2d::ZoomViewOP(stage, stage->GetStageImpl(), true)
	, m_selected(-1)
{
}

bool EditQuadOP::OnMouseLeftDown(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseLeftDown(x, y)) {
		return true;
	}
	
	Icon* icon = static_cast<StagePanel*>(m_wnd)->GetIcon();
	if (!icon) {
		return false;
	}

	m_selected = -1;

	d2d::Vector pos = m_stage->TransPosScrToProj(x, y);

	const d2d::Vector* screen = static_cast<QuadIcon*>(icon)->GetScreen();
	for (int i = 0; i < 4; ++i) {
		if (d2d::Math::getDistance(screen[i], pos) < CTRL_NODE_RADIUS) {
			m_selected = i;
		}
	}

	return false;
}

bool EditQuadOP::OnMouseLeftUp(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseLeftUp(x, y)) {
		return true;
	}

	m_selected = -1;

	return false;
}

bool EditQuadOP::OnMouseDrag(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseDrag(x, y)) {
		return true;
	}

	Icon* icon = static_cast<StagePanel*>(m_wnd)->GetIcon();
	if (m_selected == -1 || !icon) {
		return false;
	}

	QuadIcon* quad_icon = static_cast<QuadIcon*>(icon);

	d2d::Vector screen[4];
	memcpy(screen, quad_icon->GetScreen(), sizeof(screen));	
	screen[m_selected] = m_stage->TransPosScrToProj(x, y);

	quad_icon->SetScreen(screen);

	m_stage->SetCanvasDirty();

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

	QuadIcon* quad_icon = static_cast<QuadIcon*>(icon);

	std::vector<d2d::Vector> screen;
	for (int i = 0; i < 4; ++i) {
		screen.push_back(quad_icon->GetScreen()[i]);
	}

	d2d::PrimitiveDraw::drawPolyline(screen, d2d::LIGHT_GREEN, true);
	for (int i = 0; i < 4; ++i) {
		d2d::PrimitiveDraw::rect(screen[i], CTRL_NODE_RADIUS, CTRL_NODE_RADIUS, d2d::LIGHT_GREEN_FACE);
	}

	return false;
}

}