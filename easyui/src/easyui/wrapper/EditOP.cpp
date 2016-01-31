#include "EditOP.h"
#include "StagePanel.h"

namespace eui
{
namespace wrapper
{

EditOP::EditOP(StagePanel* stage, ee::PropertySettingPanel* property)
	: ee::ArrangeSpriteOP<ee::SelectSpritesOP>(stage, stage->GetStageImpl(), stage, property)
	, EditClipboxOP(stage, stage->GetStageImpl(), stage->GetClipbox())
	, m_stage(stage)
	, m_clipbox_selected(-1)
{
}

bool EditOP::OnKeyDown(int keyCode)
{
	return ee::ArrangeSpriteOP<ee::SelectSpritesOP>::OnKeyDown(keyCode);
}

bool EditOP::OnKeyUp(int keyCode)
{
	return ee::ArrangeSpriteOP<ee::SelectSpritesOP>::OnKeyUp(keyCode);	
}

bool EditOP::OnMouseLeftDown(int x, int y)
{
	if (EditClipboxOP::OnMouseLeftDown(x, y)) {
		return true;
	} else {
		return ee::ArrangeSpriteOP<ee::SelectSpritesOP>::OnMouseLeftDown(x, y);
	}
}

bool EditOP::OnMouseLeftUp(int x, int y)
{
	if (EditClipboxOP::OnMouseLeftDown(x, y)) return true;
	m_clipbox_selected = -1;
	return ee::ArrangeSpriteOP<ee::SelectSpritesOP>::OnMouseLeftUp(x, y);
}

bool EditOP::OnMouseDrag(int x, int y)
{
	if (EditClipboxOP::OnMouseDrag(x, y)) {
		return true;
	} else {
		return ee::ArrangeSpriteOP<ee::SelectSpritesOP>::OnMouseDrag(x, y);
	}
}

bool EditOP::OnDraw() const
{
	if (EditClipboxOP::OnDraw()) {
		return true;
	} else {
		return ee::ArrangeSpriteOP<ee::SelectSpritesOP>::OnDraw();
	}
}

}
}