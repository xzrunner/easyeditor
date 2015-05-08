#include "ArrangeSpriteImpl.h"
#include "StagePanel.h"
#include "RightPopupMenu.h"

namespace lr
{

ArrangeSpriteImpl::ArrangeSpriteImpl(StagePanel* stage, 
									 d2d::PropertySettingPanel* property,
									 RightPopupMenu* popup)
	: d2d::ArrangeSpriteImpl(stage, stage, property)
	, m_popup(popup)
{
}

void ArrangeSpriteImpl::SetRightPopupMenu(wxMenu& menu, d2d::ISprite* spr)
{
	d2d::ArrangeSpriteImpl::SetRightPopupMenu(menu, spr);
	m_popup->SetRightPopupMenu(menu, spr);
}

}