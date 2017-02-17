#include "EditRectOP.h"
#include "StagePanel.h"
#include "RectIcon.h"
#include "IconType.h"

#include <ee/EditPanelImpl.h>
#include <ee/panel_msg.h>
#include <ee/Image.h>
#include <ee/ImageSymbol.h>
#include <ee/color_config.h>

#include <sprite2/S2_RVG.h>
#include <SM_Calc.h>

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

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);

	sm::rect r = icon->GetRegion(1);
	if (sm::dis_pos_to_pos(sm::vec2(r.xmin, r.ymin), pos) < CTRL_NODE_RADIUS) {
		m_selected = PT_LEFT_LOW;
	} else if (sm::dis_pos_to_pos(sm::vec2(r.xmin, r.ymax), pos) < CTRL_NODE_RADIUS) {
		m_selected = PT_LEFT_TOP;
	} else if (sm::dis_pos_to_pos(sm::vec2(r.xmax, r.ymax), pos) < CTRL_NODE_RADIUS) {
		m_selected = PT_RIGHT_TOP;
	} else if (sm::dis_pos_to_pos(sm::vec2(r.xmax, r.ymin), pos) < CTRL_NODE_RADIUS) {
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

	sm::rect r = icon->GetRegion(1);

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
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
	icon->RemoveReference();	

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

	const s2::ImageSymbol* img = icon->GetImage();
	if (!img) {
		return false;
	}

	const ee::Image* ee_img = dynamic_cast<const ee::ImageSymbol*>(img)->GetImage();
	float w = ee_img->GetClippedRegion().Width(),
		  h = ee_img->GetClippedRegion().Height();
	s2::RVG::SetColor(ee::LIGHT_RED);
	s2::RVG::LineWidth(1);
	s2::RVG::Rect(sm::vec2(0, 0), w * 0.5f, h * 0.5f, false);

	sm::rect r = icon->GetRegion(1);
	s2::RVG::SetColor(ee::LIGHT_GREEN);
	s2::RVG::Rect(sm::vec2(r.xmin, r.ymin), sm::vec2(r.xmax, r.ymax), false);
	s2::RVG::Rect(sm::vec2(r.xmin, r.ymin), CTRL_NODE_RADIUS, CTRL_NODE_RADIUS, true);
	s2::RVG::Rect(sm::vec2(r.xmin, r.ymax), CTRL_NODE_RADIUS, CTRL_NODE_RADIUS, true);
	s2::RVG::Rect(sm::vec2(r.xmax, r.ymax), CTRL_NODE_RADIUS, CTRL_NODE_RADIUS, true);
	s2::RVG::Rect(sm::vec2(r.xmax, r.ymin), CTRL_NODE_RADIUS, CTRL_NODE_RADIUS, true);

	s2::RVG::LineWidth(2);

	return false;
}

}