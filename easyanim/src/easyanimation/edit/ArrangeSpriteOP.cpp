#include "ArrangeSpriteOP.h"
#include "KeyDownHandler.h"

#include "view/StagePanel.h"
#include "view/ViewMgr.h"

#include <ee/std_functor.h>
#include <ee/panel_msg.h>
#include <ee/color_config.h>
#include <ee/Math2D.h>

#include <algorithm>

namespace eanim
{

ArrangeSpriteOP::ArrangeSpriteOP(StagePanel* stage)
	: ee::ArrangeSpriteOP<SelectSpritesOP>(stage, stage->GetStageImpl(), stage, ViewMgr::Instance()->property)
	, m_selected(NULL)
{
}

ArrangeSpriteOP::~ArrangeSpriteOP()
{
	for_each(m_crosses.begin(), m_crosses.end(), ee::DeletePointerFunctor<Cross>());
}

bool ArrangeSpriteOP::OnKeyDown(int keyCode)
{
	if (ee::ArrangeSpriteOP<SelectSpritesOP>::OnKeyDown(keyCode)) 
		return true;

	return KeyDownHandler::Instance()->Process(keyCode);
}

bool ArrangeSpriteOP::OnMouseLeftDown(int x, int y)
{
	if (ee::ArrangeSpriteOP<SelectSpritesOP>::OnMouseLeftDown(x, y)) 
		return true;

	ee::Vector pos = m_stage->TransPosScrToProj(x, y);
	for (int i = 0, n = m_crosses.size(); i < n; ++i) {
		if (m_crosses[i]->Contain(pos)) {
			m_selected = m_crosses[i];
		}
	}	

	return false;
}

bool ArrangeSpriteOP::OnMouseLeftUp(int x, int y)
{
	if (ee::ArrangeSpriteOP<SelectSpritesOP>::OnMouseLeftUp(x, y)) 
		return true;

	m_selected = NULL;

	return false;
}

bool ArrangeSpriteOP::OnMouseDrag(int x, int y)
{
	if (m_selected)
	{
		m_selected->pos = m_stage->TransPosScrToProj(x, y);
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}
	else
	{
		if (ee::ArrangeSpriteOP<SelectSpritesOP>::OnMouseDrag(x, y)) 
			return true;
	}

	return false;
}

bool ArrangeSpriteOP::OnDraw() const
{
	if (ee::ArrangeSpriteOP<SelectSpritesOP>::OnDraw()) 
		return true;

	for (int i = 0, n = m_crosses.size(); i < n; ++i) {
		m_crosses[i]->Draw();
	}

	return false;
}

void ArrangeSpriteOP::AddCross()
{
	m_crosses.push_back(new Cross());
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
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

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
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
	ee::RVG::Color(ee::LIGHT_RED);
	ee::RVG::Circle(pos, RADIUS, false);
	ee::RVG::Cross(pos, LENGTH);
}

bool ArrangeSpriteOP::Cross::Contain(const ee::Vector& p) const
{
	return ee::Math2D::GetDistance(pos, p) < RADIUS;
}

} // eanim