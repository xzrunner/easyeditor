#include "ArrangeSpriteFixOP.h"

#include "view/MultiSpritesImpl.h"
#include "view/SpriteSelection.h"
#include "message/RemoveSpriteSJ.h"
#include "message/InsertSpriteSJ.h"

namespace d2d
{

ArrangeSpriteFixOP::ArrangeSpriteFixOP(wxWindow* wnd, EditPanelImpl* stage, 
									   MultiSpritesImpl* spritesImpl,
									   PropertySettingPanel* propertyPanel/* = NULL*/, 
									   AbstractEditCMPT* callback/* = NULL*/, 
									   const ArrangeSpriteConfig& cfg/* = NULL*/)
	: ArrangeSpriteOP(wnd, stage, spritesImpl, propertyPanel, callback, cfg)
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
	std::vector<ISprite*> sprites;
	m_selection->Traverse(FetchAllVisitor<ISprite>(sprites));
	fixSpritesLocation(sprites);
}

void ArrangeSpriteFixOP::fixSpritesLocation(const std::vector<ISprite*>& sprites)
{
	for (size_t i = 0, n = sprites.size(); i < n; ++i) {
		ISprite* spr = sprites[i];
		spr->Retain();
		RemoveSpriteSJ::Instance()->Remove(spr);
		InsertSpriteSJ::Instance()->Insert(spr);
		spr->Release();
	}
}

} // d2d