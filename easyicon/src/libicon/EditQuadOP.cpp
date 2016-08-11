#include "EditQuadOP.h"
#include "StagePanel.h"
#include "QuadIcon.h"
#include "IconType.h"

#include <ee/EditPanelImpl.h>
#include <ee/Math2D.h>
#include <ee/panel_msg.h>
#include <ee/color_config.h>
#include <ee/Image.h>

#include <sprite2/S2_RVG.h>

namespace eicon
{

const float EditQuadOP::CTRL_NODE_RADIUS = 5.0f;

EditQuadOP::EditQuadOP(StagePanel* stage)
	: ee::ZoomViewOP(stage, stage->GetStageImpl(), true)
	, m_selected(-1)
{
}

bool EditQuadOP::OnMouseLeftDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftDown(x, y)) {
		return true;
	}
	
	Icon* icon = static_cast<StagePanel*>(m_wnd)->GetIcon();
	if (!icon) {
		return false;
	}

	m_selected = -1;

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);

	const sm::vec2* screen = static_cast<QuadIcon*>(icon)->GetScreen();
	for (int i = 0; i < 4; ++i) {
		if (ee::Math2D::GetDistance(screen[i], pos) < CTRL_NODE_RADIUS) {
			m_selected = i;
		}
	}

	return false;
}

bool EditQuadOP::OnMouseLeftUp(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftUp(x, y)) {
		return true;
	}

	m_selected = -1;

	return false;
}

bool EditQuadOP::OnMouseDrag(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseDrag(x, y)) {
		return true;
	}

	Icon* icon = static_cast<StagePanel*>(m_wnd)->GetIcon();
	if (m_selected == -1 || !icon) {
		return false;
	}

	QuadIcon* quad_icon = static_cast<QuadIcon*>(icon);

	sm::vec2 screen[4];
	memcpy(screen, quad_icon->GetScreen(), sizeof(screen));	
	screen[m_selected] = m_stage->TransPosScrToProj(x, y);

	quad_icon->SetScreen(screen);

	ee::SetCanvasDirtySJ::Instance()->SetDirty();

	return false;
}

bool EditQuadOP::OnActive()
{
	if (ee::ZoomViewOP::OnActive()) {
		return true;
	}

	StagePanel* stage = static_cast<StagePanel*>(m_wnd);
	Icon* icon = stage->GetIcon();
	if (icon && get_icon_type(icon->GetIconDesc()) == IT_QUAD) {
		return false;
	}

	icon = new QuadIcon;
	stage->SetIcon(icon);
	icon->Release();	

	return false;
}

bool EditQuadOP::OnDraw() const
{
	if (ee::ZoomViewOP::OnDraw()) {
		return true;
	}

	Icon* icon = static_cast<StagePanel*>(m_wnd)->GetIcon();
	if (!icon) {
		return false;
	}

	const ee::Image* img = icon->GetImage();
	if (!img) {
		return false;
	}

	float w = img->GetClippedWidth(),
		h = img->GetClippedHeight();
	s2::RVG::SetColor(ee::LIGHT_RED);
	s2::RVG::LineWidth(1);
	s2::RVG::Rect(sm::vec2(0, 0), w * 0.5f, h * 0.5f, false);
	s2::RVG::LineWidth(2);

	QuadIcon* quad_icon = static_cast<QuadIcon*>(icon);

	std::vector<sm::vec2> screen;
	for (int i = 0; i < 4; ++i) {
		screen.push_back(quad_icon->GetScreen()[i]);
	}

	s2::RVG::SetColor(ee::LIGHT_GREEN);
	s2::RVG::Polyline(screen, true);
	for (int i = 0; i < 4; ++i) {
		s2::RVG::SetColor(ee::LIGHT_GREEN);
		s2::RVG::Rect(screen[i], CTRL_NODE_RADIUS, CTRL_NODE_RADIUS, true);
	}

	return false;
}

}