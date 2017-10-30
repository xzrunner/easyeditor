#include "ArrangeSpriteFixOP.h"
#include "MultiSpritesImpl.h"
#include "SpriteSelection.h"
#include "FetchAllVisitor.h"
#include "sprite_msg.h"

namespace ee
{

ArrangeSpriteFixOP::ArrangeSpriteFixOP(wxWindow* wnd, EditPanelImpl* stage, 
									   MultiSpritesImpl* sprites_impl,
									   PropertySettingPanel* property/* = NULL*/, 
									   EditCMPT* callback/* = NULL*/, 
									   const ArrangeSpriteConfig& cfg/* = NULL*/)
	: ArrangeSpriteOP(wnd, stage, sprites_impl, property, callback, cfg)
{
}

bool ArrangeSpriteFixOP::OnMouseLeftUp(int x, int y)
{
	if (ArrangeSpriteOP::OnMouseLeftUp(x, y)) return true;
	FixSpritesLocation();
	return false;
}

// bool ArrangeSpriteFixOP::OnMouseRightDown(int x, int y)
// {
// 	if (ArrangeSpriteOP::OnMouseRightDown(x, y)) return true;
// 
// 	OnMouseLeftDown(x, y);
// 
// 	return false;
// }

bool ArrangeSpriteFixOP::OnMouseRightUp(int x, int y)
{
	if (ArrangeSpriteOP::OnMouseRightUp(x, y)) return true;
	FixSpritesLocation();
	return false;
}

void ArrangeSpriteFixOP::FixSpritesLocation()
{
	std::vector<SprPtr> sprs;
	m_selection->Traverse(FetchAllRefVisitor<Sprite>(sprs));
	FixSpritesLocation(sprs);
}

void ArrangeSpriteFixOP::FixSpritesLocation(const std::vector<SprPtr>& sprs)
{
	for (auto& spr : sprs) 
	{
		RemoveSpriteSJ::Instance()->Remove(spr);
		InsertSpriteSJ::Instance()->Insert(spr);
	}
}

}