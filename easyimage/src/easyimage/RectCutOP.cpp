#include "RectCutOP.h"
#include "StagePanel.h"

#include <ee/panel_msg.h>
#include <ee/Math2D.h>
#include <ee/EE_RVG.h>
#include <ee/color_config.h>
#include <ee/Sprite.h>
#include <ee/Symbol.h>

namespace eimage
{

RectCutOP::RectCutOP(RectCutCMPT* cmpt, StagePanel* stage)
	: ee::ZoomViewOP(stage, stage->GetStageImpl(), true)
	, m_cmpt(cmpt)
	, m_stage(stage)
	, m_rect_selected(NULL)
	, m_first_pos_valid(false)
	, m_curr_pos_valid(false)
	, m_captured_valid(false)
{
}

bool RectCutOP::OnMouseLeftDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	if (!m_stage->GetImage()) return false;

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	m_node_selected = m_rects.QueryNode(pos);

	return false;
}

bool RectCutOP::OnMouseLeftUp(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftUp(x, y)) return true;

	if (!m_stage->GetImage()) return false;

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
		m_rect_selected->xmin = ceil(m_rect_selected->xmin);
		m_rect_selected->xmax = ceil(m_rect_selected->xmax);
		m_rect_selected->ymin = ceil(m_rect_selected->ymin);
		m_rect_selected->ymax = ceil(m_rect_selected->ymax);
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}

	return false;
}

bool RectCutOP::OnMouseRightDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	if (!m_stage->GetImage()) return false;

	m_first_pos = m_stage->TransPosScrToProj(x, y);
	m_first_pos_valid = true;
	FixedPos(m_first_pos);

	return false;
}

bool RectCutOP::OnMouseRightUp(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftUp(x, y)) return true;

	if (!m_stage->GetImage()) return false;

	if (!m_first_pos_valid) {
		return false;
	}

	const float RADIUS = 5;
	// remove rect
	m_curr_pos = m_stage->TransPosScrToProj(x, y);
	m_curr_pos_valid = true;
	if (ee::Math2D::GetDistance(m_curr_pos, m_first_pos) < RADIUS)
	{
		bool removed = m_rects.Remove(m_curr_pos);
		if (removed) {
			m_node_selected.rect = NULL;
			m_rect_selected = NULL;

			m_first_pos_valid = false;
			m_curr_pos_valid = false;
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

			m_first_pos_valid = false;
			m_curr_pos_valid = false;
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
	m_curr_pos_valid = true;
	m_rect_selected = m_rects.QueryRect(m_curr_pos);
	m_captured = m_rects.QueryNearestAxis(m_curr_pos);
	m_captured_valid = true;
	ee::SetCanvasDirtySJ::Instance()->SetDirty();

	return false;
}

bool RectCutOP::OnMouseDrag(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseDrag(x, y)) return true;

	if (!m_stage->GetImage()) return false;

	// create rect
	if (m_first_pos_valid)
	{
		m_curr_pos = m_stage->TransPosScrToProj(x, y);
		m_curr_pos_valid = true;
		m_captured = m_rects.QueryNearestAxis(m_curr_pos);
		m_captured_valid = true;

		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}
	// move rect's node
	else if (m_node_selected.rect)
	{
		m_curr_pos = m_stage->TransPosScrToProj(x, y);
		m_curr_pos_valid = true;
		m_captured = m_rects.QueryNearestAxis(m_curr_pos, m_node_selected.rect);
		m_captured_valid = true;

		sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
		m_rects.MoveNode(m_node_selected, pos);
		m_node_selected.pos = pos;

		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}
	// move rect
	else if (m_rect_selected)
	{
		sm::vec2 curr = m_stage->TransPosScrToProj(x, y);
		m_rects.MoveRect(m_rect_selected, m_curr_pos, curr);
		m_curr_pos = curr;
		m_curr_pos_valid = true;

		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}

	return false;
}

bool RectCutOP::OnDraw() const
{
	if (ee::ZoomViewOP::OnDraw()) return true;

	ee::RVG::Color(s2::Color(255, 0, 0));
	ee::RVG::Cross(sm::vec2(0, 0), 100, 100);

	if (!m_stage->GetImage()) return false;

	m_rects.Draw();

	if (m_first_pos_valid && m_curr_pos_valid)
	{
		ee::RVG::Color(ee::LIGHT_RED);
		ee::RVG::Rect(m_first_pos, m_curr_pos, false);
	}

	DrawCaptureLine();

	if (m_rect_selected) {
		ee::RVG::Color(ee::LIGHT_GREEN);
		ee::RVG::Rect(sm::vec2(m_rect_selected->xmin, m_rect_selected->ymin), 
			sm::vec2(m_rect_selected->xmax, m_rect_selected->ymax), true);
	}
	if (m_node_selected.rect) {
		ee::RVG::Color(ee::LIGHT_GREEN);
		ee::RVG::Rect(sm::vec2(m_node_selected.rect->xmin, m_node_selected.rect->ymin), 
			sm::vec2(m_node_selected.rect->ymin, m_node_selected.rect->ymax), true);
	}

	return false;
}

bool RectCutOP::Clear()
{
	if (ee::ZoomViewOP::Clear()) return true;

	m_first_pos_valid = m_curr_pos_valid = false;

	m_rects.Clear();
	m_rect_selected = NULL;
	m_node_selected.rect = NULL;

	return false;
}

std::string RectCutOP::GetImageFilepath() const
{
	if (const ee::Sprite* s = m_stage->GetImage()) {
		return s->GetSymbol().GetFilepath();
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
	if (!m_curr_pos_valid || !m_captured_valid) return;

	const float EDGE = 4096;
//	if (m_captured.x != ee::FLT_INVALID)
	if (m_captured.x != FLT_MAX)
	{
		sm::vec2 p0(m_captured.x, -EDGE);
		sm::vec2 p1(m_captured.x, EDGE);
		ee::RVG::Color(s2::Color(0, 0, 0));
		ee::RVG::DashLine(p0, p1);
	}

//	if (m_captured.y != ee::FLT_INVALID)
	if (m_captured.y != FLT_MAX)
	{
		sm::vec2 p0(-EDGE, m_captured.y);
		sm::vec2 p1(EDGE, m_captured.y);
		ee::RVG::Color(s2::Color(0, 0, 0));
		ee::RVG::DashLine(p0, p1);
	}
}

void RectCutOP::FixedPos(sm::vec2& pos) const
{
	const float RADIUS = 5;
	if (fabs(pos.x - m_captured.x) > RADIUS || 
		fabs(pos.y - m_captured.y) > RADIUS) {
		return;
	}
	
	// by capture
//	if (m_captured.x != ee::FLT_INVALID) {
	if (m_captured.x != FLT_MAX) {
		pos.x = m_captured.x;
	}
//	if (m_captured.y != ee::FLT_INVALID) {
	if (m_captured.y != FLT_MAX) {
		pos.y = m_captured.y;
	}

	// to int
	pos.x = std::ceil(pos.x);
	pos.y = std::ceil(pos.y);

	// to image
	float w = m_stage->GetImage()->GetSymbol().GetSize().Width();
	float h = m_stage->GetImage()->GetSymbol().GetSize().Height();
	if (pos.x < 0) {
		pos.x = 0;
	}
	if (pos.x > w) {
		pos.x = w;
	}
	if (pos.y < 0) {
		pos.y = 0;
	}
	if (pos.y > h) {
		pos.y = h;
	}
}

}