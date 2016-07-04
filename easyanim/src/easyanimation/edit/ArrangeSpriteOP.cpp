#include "ArrangeSpriteOP.h"
#include "KeyDownHandler.h"

#include "view/StagePanel.h"
#include "view/ViewMgr.h"

#include <ee/panel_msg.h>

namespace eanim
{

ArrangeSpriteOP::ArrangeSpriteOP(StagePanel* stage)
	: ee::ArrangeSpriteOP<SelectSpritesOP>(stage, stage->GetStageImpl(), stage, ViewMgr::Instance()->property)
{
}

bool ArrangeSpriteOP::OnKeyDown(int keyCode)
{
	if (ee::ArrangeSpriteOP<SelectSpritesOP>::OnKeyDown(keyCode)) 
		return true;

	return KeyDownHandler::Instance()->Process(keyCode);
}

bool ArrangeSpriteOP::OnMouseLeftDown(int x, int y)
{
	if (m_guides.OnMouseDown(m_stage->TransPosScrToProj(x, y))) {
		return false;
	} else {
		return ee::ArrangeSpriteOP<SelectSpritesOP>::OnMouseLeftDown(x, y);
	}
}

bool ArrangeSpriteOP::OnMouseLeftUp(int x, int y)
{
	bool ret = ee::ArrangeSpriteOP<SelectSpritesOP>::OnMouseLeftUp(x, y);
	m_guides.OnMouseUp(m_stage->TransPosScrToProj(x, y));
	return ret;
}

bool ArrangeSpriteOP::OnMouseDrag(int x, int y)
{
	if (m_guides.OnMouseDrag(m_stage->TransPosScrToProj(x, y))) {
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
		return false;
	} else {
		return ee::ArrangeSpriteOP<SelectSpritesOP>::OnMouseDrag(x, y);
	}
}

bool ArrangeSpriteOP::OnDraw() const
{
	bool ret = ee::ArrangeSpriteOP<SelectSpritesOP>::OnDraw();
	m_guides.Draw();
	return ret;
}

void ArrangeSpriteOP::AddCross()
{
	m_guides.Add();
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void ArrangeSpriteOP::DelCross()
{
	m_guides.Remove();
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

} // eanim