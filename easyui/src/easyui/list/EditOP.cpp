#include "EditOP.h"
#include "StagePanel.h"

namespace eui
{
namespace list
{

EditOP::EditOP(StagePanel* stage, d2d::PropertySettingPanel* property, d2d::ViewPanelMgr* view_panel_mgr)
	: d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>(stage, stage->GetStageImpl(), stage, property, view_panel_mgr)
	, m_stage(stage)
	, m_clipbox_selected(-1)
{
}

bool EditOP::OnMouseLeftDown(int x, int y)
{
	d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
	const d2d::Rect& r = m_stage->GetList().GetClipbox();

	m_clipbox_selected = -1;
	if (d2d::Math::getDistance(d2d::Vector(r.xMin, r.yMin), pos) < NODE_RADIUS) {
		m_clipbox_selected = 0;
	} else if (d2d::Math::getDistance(d2d::Vector(r.xMin, r.yMax), pos) < NODE_RADIUS) {
		m_clipbox_selected = 1;
	} else if (d2d::Math::getDistance(d2d::Vector(r.xMax, r.yMax), pos) < NODE_RADIUS) {
		m_clipbox_selected = 2;
	} else if (d2d::Math::getDistance(d2d::Vector(r.xMax, r.yMin), pos) < NODE_RADIUS) {
		m_clipbox_selected = 3;
	}

	if (m_clipbox_selected == -1) {
		return d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>::OnMouseLeftDown(x, y);
	} else {
		return false;
	}
}

bool EditOP::OnMouseLeftUp(int x, int y)
{
	m_clipbox_selected = -1;
	return d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>::OnMouseLeftUp(x, y);
}

bool EditOP::OnMouseDrag(int x, int y)
{
	if (m_clipbox_selected != -1) {
		d2d::Rect r = m_stage->GetList().GetClipbox();
		d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
		if (m_clipbox_selected == 0) {
			r.xMin = pos.x;			
			r.yMin = pos.y;
		} else if (m_clipbox_selected == 1) {
			r.xMin = pos.x;			
			r.yMax = pos.y;
		} else if (m_clipbox_selected == 2) {
			r.xMax = pos.x;			
			r.yMax = pos.y;
		} else if (m_clipbox_selected == 3) {
			r.xMax = pos.x;
			r.yMin = pos.y;
		}
		m_stage->GetList().SetClipbox(r);
		m_stage->SetCanvasDirty();
		return false;
	} else {
		return d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>::OnMouseDrag(x, y);
	}
}

bool EditOP::OnDraw() const
{
	if (d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>::OnDraw()) {
		return true;
	}

	const d2d::Rect& r = m_stage->GetList().GetClipbox();
	d2d::PrimitiveDraw::rect(r, d2d::LIGHT_GREEN_LINE);

	d2d::PrimitiveDraw::drawCircle(d2d::Vector(r.xMin, r.yMin), NODE_RADIUS, true, 2, d2d::LIGHT_GREY);
	d2d::PrimitiveDraw::drawCircle(d2d::Vector(r.xMin, r.yMax), NODE_RADIUS, true, 2, d2d::LIGHT_GREY);
	d2d::PrimitiveDraw::drawCircle(d2d::Vector(r.xMax, r.yMax), NODE_RADIUS, true, 2, d2d::LIGHT_GREY);
	d2d::PrimitiveDraw::drawCircle(d2d::Vector(r.xMax, r.yMin), NODE_RADIUS, true, 2, d2d::LIGHT_GREY);

	return false;
}

}
}