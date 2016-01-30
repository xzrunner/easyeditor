#include "EditRectOP.h"
#include "StagePanel.h"
#include "RectIcon.h"
#include "IconType.h"

namespace eicon
{

const float EditRectOP::CTRL_NODE_RADIUS = 5.0f;

EditRectOP::EditRectOP(StagePanel* stage)
	: d2d::ZoomViewOP(stage, stage->GetStageImpl(), true)
	, m_selected(PT_NULL)
{
}

bool EditRectOP::OnMouseLeftDown(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseLeftDown(x, y)) {
		return true;
	}

	Icon* icon = static_cast<StagePanel*>(m_wnd)->GetIcon();
	if (!icon) {
		return false;
	}

	m_selected = PT_NULL;

	d2d::Vector pos = m_stage->TransPosScrToProj(x, y);

	d2d::Rect r = icon->GetRegion(1);
	if (d2d::Math2D::GetDistance(d2d::Vector(r.xmin, r.ymin), pos) < CTRL_NODE_RADIUS) {
		m_selected = PT_LEFT_LOW;
	} else if (d2d::Math2D::GetDistance(d2d::Vector(r.xmin, r.ymax), pos) < CTRL_NODE_RADIUS) {
		m_selected = PT_LEFT_TOP;
	} else if (d2d::Math2D::GetDistance(d2d::Vector(r.xmax, r.ymax), pos) < CTRL_NODE_RADIUS) {
		m_selected = PT_RIGHT_TOP;
	} else if (d2d::Math2D::GetDistance(d2d::Vector(r.xmax, r.ymin), pos) < CTRL_NODE_RADIUS) {
		m_selected = PT_RIGHT_LOW;
	}

	return false;
}

bool EditRectOP::OnMouseLeftUp(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseLeftUp(x, y)) {
		return true;
	}

	m_selected = PT_NULL;

	return false;
}

bool EditRectOP::OnMouseDrag(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseDrag(x, y)) {
		return true;
	}

	Icon* icon = static_cast<StagePanel*>(m_wnd)->GetIcon();
	if (m_selected == PT_NULL || !icon) {
		return false;
	}

	d2d::Rect r = icon->GetRegion(1);

	d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
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

	d2d::SetCanvasDirtySJ::Instance()->SetDirty();

	return false;
}

bool EditRectOP::OnActive()
{
	if (d2d::ZoomViewOP::OnActive()) {
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
	d2d::PrimitiveDraw::DrawRect(d2d::Vector(0, 0), w * 0.5f, h * 0.5f, 
		d2d::LIGHT_RED_THIN_LINE);

	d2d::Rect r = icon->GetRegion(1);
	d2d::PrimitiveDraw::DrawRect(r, d2d::LIGHT_GREEN_THIN_LINE);
	d2d::PrimitiveDraw::DrawRect(d2d::Vector(r.xmin, r.ymin), CTRL_NODE_RADIUS, CTRL_NODE_RADIUS, d2d::LIGHT_GREEN_FACE);
	d2d::PrimitiveDraw::DrawRect(d2d::Vector(r.xmin, r.ymax), CTRL_NODE_RADIUS, CTRL_NODE_RADIUS, d2d::LIGHT_GREEN_FACE);
	d2d::PrimitiveDraw::DrawRect(d2d::Vector(r.xmax, r.ymax), CTRL_NODE_RADIUS, CTRL_NODE_RADIUS, d2d::LIGHT_GREEN_FACE);
	d2d::PrimitiveDraw::DrawRect(d2d::Vector(r.xmax, r.ymin), CTRL_NODE_RADIUS, CTRL_NODE_RADIUS, d2d::LIGHT_GREEN_FACE);
	
	return false;
}

}