#include "ArrangeSpriteOP.h"
#include "KeyDownHandler.h"

#include "view/StagePanel.h"
#include "view/ViewMgr.h"

namespace eanim
{

ArrangeSpriteOP::ArrangeSpriteOP(StagePanel* stage)
	: d2d::ArrangeSpriteOP<SelectSpritesOP>(stage, stage->GetStageImpl(), stage, ViewMgr::Instance()->property)
	, m_selected(NULL)
{
}

ArrangeSpriteOP::~ArrangeSpriteOP()
{
	for_each(m_crosses.begin(), m_crosses.end(), DeletePointerFunctor<Cross>());
}

bool ArrangeSpriteOP::OnKeyDown(int keyCode)
{
	if (d2d::ArrangeSpriteOP<SelectSpritesOP>::OnKeyDown(keyCode)) 
		return true;

	return KeyDownHandler::Instance()->Process(keyCode);
}

bool ArrangeSpriteOP::OnMouseLeftDown(int x, int y)
{
	if (d2d::ArrangeSpriteOP<SelectSpritesOP>::OnMouseLeftDown(x, y)) 
		return true;

	d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
	for (int i = 0, n = m_crosses.size(); i < n; ++i) {
		if (m_crosses[i]->Contain(pos)) {
			m_selected = m_crosses[i];
		}
	}	

	return false;
}

bool ArrangeSpriteOP::OnMouseLeftUp(int x, int y)
{
	if (d2d::ArrangeSpriteOP<SelectSpritesOP>::OnMouseLeftUp(x, y)) 
		return true;

	m_selected = NULL;

	return false;
}

bool ArrangeSpriteOP::OnMouseDrag(int x, int y)
{
	if (m_selected)
	{
		m_selected->pos = m_stage->TransPosScrToProj(x, y);
		d2d::SetCanvasDirtySJ::Instance()->SetDirty();
	}
	else
	{
		if (d2d::ArrangeSpriteOP<SelectSpritesOP>::OnMouseDrag(x, y)) 
			return true;
	}

	return false;
}

bool ArrangeSpriteOP::OnDraw() const
{
	if (d2d::ArrangeSpriteOP<SelectSpritesOP>::OnDraw()) 
		return true;

	for (int i = 0, n = m_crosses.size(); i < n; ++i) {
		m_crosses[i]->Draw();
	}

	return false;
}

void ArrangeSpriteOP::AddCross()
{
	m_crosses.push_back(new Cross());
	d2d::SetCanvasDirtySJ::Instance()->SetDirty();
}

void ArrangeSpriteOP::DelCross()
{
	if (m_crosses.empty())
		return;

	Cross* del = m_crosses.back();
	m_crosses.pop_back();
	if (m_selected == del)
	{
		if (m_crosses.empty()) {
			m_selected = NULL;
		} else {
			m_selected = m_crosses.back();
		}
	}
	delete del;

	d2d::SetCanvasDirtySJ::Instance()->SetDirty();
}

//////////////////////////////////////////////////////////////////////////
// class ArrangeSpriteOP::Cross
//////////////////////////////////////////////////////////////////////////

ArrangeSpriteOP::Cross::Cross()
	: pos(0, 0)
{
}

void ArrangeSpriteOP::Cross::Draw() const
{
	d2d::PrimitiveDraw::DrawCircle(pos, RADIUS, false, 1, d2d::LIGHT_RED);
	d2d::PrimitiveDraw::Cross(pos, LENGTH, d2d::LIGHT_RED, 1);
}

bool ArrangeSpriteOP::Cross::Contain(const d2d::Vector& p) const
{
	return d2d::Math2D::GetDistance(pos, p) < RADIUS;
}

} // eanim