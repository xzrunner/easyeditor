#include "RectCutOP.h"
#include "StagePanel.h"

#include <ee/panel_msg.h>
#include <ee/Math2D.h>
#include <ee/color_config.h>
#include <ee/Sprite.h>
#include <ee/Symbol.h>

#include <sprite2/S2_RVG.h>

namespace eimage
{

RectCutOP::RectCutOP(RectCutCMPT* cmpt, StagePanel* stage)
	: ee::ZoomViewOP(stage, stage->GetStageImpl(), true)
	, m_cmpt(cmpt)
	, m_stage(stage)
	, m_rect_selected(NULL)
{
	m_first_pos.MakeInvalid();
	m_curr_pos.MakeInvalid();
	m_captured.MakeInvalid();
}

bool RectCutOP::OnMouseLeftDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	if (!m_stage->GetImage()) return false;

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	if (m_center.Contain(pos)) {
		m_center.selected = true;
	} else {
		m_node_selected = m_rects.QueryNode(pos);
	}

	return false;
}

bool RectCutOP::OnMouseLeftUp(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftUp(x, y)) return true;

	if (!m_stage->GetImage()) return false;

	m_center.selected = false;

	// move node
	if (m_node_selected.rect)
	{
		FixedPos(m_curr_pos);
		bool moved = m_rects.MoveNode(m_node_selected, m_curr_pos);
		if (moved)
		{
			m_node_selected.pos = m_curr_pos;
			ee::SetCanvasDirtySJ::Instance()->SetDirty();
		}
		m_node_selected.rect = NULL;
	}
	// fix rect
	if (m_rect_selected)
	{
		m_rect_selected->xmin = std::floor(m_rect_selected->xmin + 0.5f);
		m_rect_selected->xmax = std::floor(m_rect_selected->xmax + 0.5f);
		m_rect_selected->ymin = std::floor(m_rect_selected->ymin + 0.5f);
		m_rect_selected->ymax = std::floor(m_rect_selected->ymax + 0.5f);
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}

	return false;
}

bool RectCutOP::OnMouseRightDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	if (!m_stage->GetImage()) return false;

	m_first_pos = m_stage->TransPosScrToProj(x, y);
	FixedPos(m_first_pos);

	return false;
}

bool RectCutOP::OnMouseRightUp(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftUp(x, y)) return true;

	if (!m_stage->GetImage()) return false;

	if (!m_first_pos.IsValid()) {
		return false;
	}

	const float RADIUS = 5;
	// remove rect
	m_curr_pos = m_stage->TransPosScrToProj(x, y);
	if (ee::Math2D::GetDistance(m_curr_pos, m_first_pos) < RADIUS)
	{
		bool removed = m_rects.Remove(m_curr_pos);
		if (removed) {
			m_node_selected.rect = NULL;
			m_rect_selected = NULL;

			m_first_pos.MakeInvalid();
			m_curr_pos.MakeInvalid();

			ee::SetCanvasDirtySJ::Instance()->SetDirty();
		}
	}
	// insert rect
	else
	{
		FixedPos(m_curr_pos);
		if (m_first_pos.x != m_curr_pos.x && m_first_pos.y != m_curr_pos.y)
		{
			m_rects.Insert(sm::rect(m_first_pos, m_curr_pos));

			m_first_pos.MakeInvalid();
			m_curr_pos.MakeInvalid();

			ee::SetCanvasDirtySJ::Instance()->SetDirty();
		}
	}

	return false;
}

bool RectCutOP::OnMouseMove(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseMove(x, y)) return true;

	if (!m_stage->GetImage()) return false;

	m_curr_pos = m_stage->TransPosScrToProj(x, y);
	m_rect_selected = m_rects.QueryRect(m_curr_pos);
	m_captured = m_rects.QueryNearestAxis(m_curr_pos);

	ee::SetCanvasDirtySJ::Instance()->SetDirty();

	return false;
}

bool RectCutOP::OnMouseDrag(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseDrag(x, y)) return true;

	if (!m_stage->GetImage()) return false;

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	// move center
	if (m_center.selected)
	{
		m_center.pos = pos;
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}
	// create rect
	else if (m_first_pos.IsValid())
	{
		m_curr_pos = pos;
		m_captured = m_rects.QueryNearestAxis(m_curr_pos);

		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}
	// move rect edge
	else if (m_node_selected.rect)
	{
		m_curr_pos = pos;
		m_captured = m_rects.QueryNearestAxis(m_curr_pos, m_node_selected.rect);

		m_rects.MoveNode(m_node_selected, pos);
		m_node_selected.pos = pos;

		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}
	// move rect
	else if (m_rect_selected)
	{
		sm::vec2 curr = pos;
		m_rects.MoveRect(m_rect_selected, m_curr_pos, curr);
		m_curr_pos = curr;

		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}

	return false;
}

bool RectCutOP::OnMouseLeftDClick(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftDClick(x, y)) return true;

	if (!m_stage->GetImage()) return false;

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	sm::rect* rect = m_rects.QueryRect(pos);
	m_center.pos = rect->Center();

	return false;
}

bool RectCutOP::OnDraw() const
{
	if (ee::ZoomViewOP::OnDraw()) return true;

	s2::RVG::SetColor(s2::Color(255, 0, 0));
	s2::RVG::Cross(sm::vec2(0, 0), 100, 100);

	if (!m_stage->GetImage()) return false;

	m_rects.Draw();

	if (m_first_pos.IsValid() && m_curr_pos.IsValid())
	{
		s2::RVG::SetColor(ee::LIGHT_RED);
		s2::RVG::Rect(m_first_pos, m_curr_pos, false);
	}

	DrawCaptureLine();

	if (m_rect_selected) {
		s2::RVG::SetColor(ee::LIGHT_GREEN);
		s2::RVG::Rect(sm::vec2(m_rect_selected->xmin, m_rect_selected->ymin), 
			sm::vec2(m_rect_selected->xmax, m_rect_selected->ymax), true);
	}
	if (m_node_selected.rect) {
		s2::RVG::SetColor(ee::LIGHT_GREEN);
		s2::RVG::Rect(sm::vec2(m_node_selected.rect->xmin, m_node_selected.rect->ymin), 
			sm::vec2(m_node_selected.rect->xmax, m_node_selected.rect->ymax), true);
	}

	m_center.Draw();

	return false;
}

bool RectCutOP::Clear()
{
	if (ee::ZoomViewOP::Clear()) return true;

	m_first_pos.MakeInvalid();
	m_curr_pos.MakeInvalid();

	m_rects.Clear();
	m_rect_selected = NULL;
	m_node_selected.rect = NULL;

	return false;
}

std::string RectCutOP::GetImageFilepath() const
{
	if (const ee::Sprite* s = m_stage->GetImage()) {
		return s->GetSymbol()->GetFilepath();
	} else {
		return "";
	}
}

void RectCutOP::LoadImageFromFile(const std::string& filepath)
{
	m_stage->SetImage(filepath);
}

void RectCutOP::DrawCaptureLine() const
{
	if (!m_curr_pos.IsValid() || !m_captured.IsValid()) return;

	const float EDGE = 4096;
	if (m_captured.x != FLT_MAX)
	{
		sm::vec2 p0(m_captured.x, -EDGE);
		sm::vec2 p1(m_captured.x, EDGE);
		s2::RVG::SetColor(s2::Color(0, 0, 0));
		s2::RVG::DashLine(p0, p1);
	}

	if (m_captured.y != FLT_MAX)
	{
		sm::vec2 p0(-EDGE, m_captured.y);
		sm::vec2 p1(EDGE, m_captured.y);
		s2::RVG::SetColor(s2::Color(0, 0, 0));
		s2::RVG::DashLine(p0, p1);
	}
}

void RectCutOP::FixedPos(sm::vec2& pos) const
{
	// to int
	pos.x = std::floor(pos.x + 0.5f);
	pos.y = std::floor(pos.y + 0.5f);

	// to image
	sm::vec2 sz = m_stage->GetImage()->GetSymbol()->GetBounding().Size();
	if (pos.x < 0) {
		pos.x = 0;
	}
	if (pos.x > sz.x) {
		pos.x = sz.x;
	}
	if (pos.y < 0) {
		pos.y = 0;
	}
	if (pos.y > sz.y) {
		pos.y = sz.y;
	}

	// to capture
	const float RADIUS = 5;
	if (fabs(pos.x - m_captured.x) <= RADIUS &&
		fabs(pos.y - m_captured.y) <= RADIUS) 
	{
		if (m_captured.x != FLT_MAX) {
			pos.x = m_captured.x;
		}
		if (m_captured.y != FLT_MAX) {
			pos.y = m_captured.y;
		}
	}	
}

//////////////////////////////////////////////////////////////////////////
// class RectCutOP::Cross
//////////////////////////////////////////////////////////////////////////

RectCutOP::Cross::Cross()
	: pos(0, 0)
	, selected(false)
{
}

void RectCutOP::Cross::Draw() const
{
	s2::RVG::SetColor(ee::BLACK);
	s2::RVG::Circle(pos, RADIUS, false);
	s2::RVG::Cross(pos, LENGTH);
}

bool RectCutOP::Cross::Contain(const sm::vec2& p) const
{
	return ee::Math2D::GetDistance(pos, p) < RADIUS;
}

}