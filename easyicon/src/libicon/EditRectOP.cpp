#include "EditRectOP.h"
#include "StagePanel.h"
#include "RectIcon.h"
#include "IconType.h"

#include <ee/EditPanelImpl.h>
#include <ee/Math2D.h>
#include <ee/panel_msg.h>
#include <ee/Image.h>
#include <ee/EE_RVG.h>
#include <ee/style_config.h>

namespace eicon
{

const float EditRectOP::CTRL_NODE_RADIUS = 5.0f;

EditRectOP::EditRectOP(StagePanel* stage)
	: ee::ZoomViewOP(stage, stage->GetStageImpl(), true)
	, m_selected(PT_NULL)
{
}

bool EditRectOP::OnMouseLeftDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftDown(x, y)) {
		return true;
	}

	Icon* icon = static_cast<StagePanel*>(m_wnd)->GetIcon();
	if (!icon) {
		return false;
	}

	m_selected = PT_NULL;

	ee::Vector pos = m_stage->TransPosScrToProj(x, y);

	ee::Rect r = icon->GetRegion(1);
	if (ee::Math2D::GetDistance(ee::Vector(r.xmin, r.ymin), pos) < CTRL_NODE_RADIUS) {
		m_selected = PT_LEFT_LOW;
	} else if (ee::Math2D::GetDistance(ee::Vector(r.xmin, r.ymax), pos) < CTRL_NODE_RADIUS) {
		m_selected = PT_LEFT_TOP;
	} else if (ee::Math2D::GetDistance(ee::Vector(r.xmax, r.ymax), pos) < CTRL_NODE_RADIUS) {
		m_selected = PT_RIGHT_TOP;
	} else if (ee::Math2D::GetDistance(ee::Vector(r.xmax, r.ymin), pos) < CTRL_NODE_RADIUS) {
		m_selected = PT_RIGHT_LOW;
	}

	return false;
}

bool EditRectOP::OnMouseLeftUp(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftUp(x, y)) {
		return true;
	}

	m_selected = PT_NULL;

	return false;
}

bool EditRectOP::OnMouseDrag(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseDrag(x, y)) {
		return true;
	}

	Icon* icon = static_cast<StagePanel*>(m_wnd)->GetIcon();
	if (m_selected == PT_NULL || !icon) {
		return false;
	}

	ee::Rect r = icon->GetRegion(1);

	ee::Vector pos = m_stage->TransPosScrToProj(x, y);
	if (m_selected == PT_LEFT_LOW) {
		r.xmin = pos.x;
		r.ymin = pos.y;
	} else if (m_selected == PT_LEFT_TOP) {
		r.xmin = pos.x;
		r.ymax = pos.y;
	} else if (m_selected == PT_RIGHT_TOP) {
		r.xmax = pos.x;
		r.ymax = pos.y;
	} else if (m_selected == PT_RIGHT_LOW) {
		r.xmax = pos.x;
		r.ymin = pos.y;
	}

	static_cast<RectIcon*>(icon)->SetRegion(r);

	ee::SetCanvasDirtySJ::Instance()->SetDirty();

	return false;
}

bool EditRectOP::OnActive()
{
	if (ee::ZoomViewOP::OnActive()) {
		return true;
	}

	StagePanel* stage = static_cast<StagePanel*>(m_wnd);
	Icon* icon = stage->GetIcon();
	if (icon && get_icon_type(icon->GetIconDesc()) == IT_RECT) {
		return false;
	}

	icon = new RectIcon;
	stage->SetIcon(icon);
	icon->Release();	

	return false;
}

bool EditRectOP::OnDraw() const
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
	ee::RVG::Rect(ee::Vector(0, 0), w * 0.5f, h * 0.5f, 
		ee::LIGHT_RED_THIN_LINE);

	ee::Rect r = icon->GetRegion(1);
	ee::RVG::Rect(ee::Vector(r.xmin, r.ymin), ee::Vector(r.xmax, r.ymax), ee::LIGHT_GREEN_THIN_LINE);
	ee::RVG::Rect(ee::Vector(r.xmin, r.ymin), CTRL_NODE_RADIUS, CTRL_NODE_RADIUS, ee::LIGHT_GREEN_FACE);
	ee::RVG::Rect(ee::Vector(r.xmin, r.ymax), CTRL_NODE_RADIUS, CTRL_NODE_RADIUS, ee::LIGHT_GREEN_FACE);
	ee::RVG::Rect(ee::Vector(r.xmax, r.ymax), CTRL_NODE_RADIUS, CTRL_NODE_RADIUS, ee::LIGHT_GREEN_FACE);
	ee::RVG::Rect(ee::Vector(r.xmax, r.ymin), CTRL_NODE_RADIUS, CTRL_NODE_RADIUS, ee::LIGHT_GREEN_FACE);
	
	return false;
}

}