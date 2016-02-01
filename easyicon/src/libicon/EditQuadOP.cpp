#include "EditQuadOP.h"
#include "StagePanel.h"
#include "QuadIcon.h"
#include "IconType.h"

#include <ee/EditPanelImpl.h>
#include <ee/Math2D.h>
#include <ee/panel_msg.h>
#include <ee/PrimitiveDraw.h>
#include <ee/style_config.h>
#include <ee/Image.h>

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

	ee::Vector pos = m_stage->TransPosScrToProj(x, y);

	const ee::Vector* screen = static_cast<QuadIcon*>(icon)->GetScreen();
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

	ee::Vector screen[4];
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
	ee::PrimitiveDraw::DrawRect(ee::Vector(0, 0), w * 0.5f, h * 0.5f, 
		ee::LIGHT_RED_THIN_LINE);

	QuadIcon* quad_icon = static_cast<QuadIcon*>(icon);

	std::vector<ee::Vector> screen;
	for (int i = 0; i < 4; ++i) {
		screen.push_back(quad_icon->GetScreen()[i]);
	}

	ee::PrimitiveDraw::DrawPolyline(screen, ee::LIGHT_GREEN, true);
	for (int i = 0; i < 4; ++i) {
		ee::PrimitiveDraw::DrawRect(screen[i], CTRL_NODE_RADIUS, CTRL_NODE_RADIUS, ee::LIGHT_GREEN_FACE);
	}

	return false;
}

}