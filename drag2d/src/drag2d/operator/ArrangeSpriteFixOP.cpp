#include "ArrangeSpriteFixOP.h"

#include "view/MultiSpritesImpl.h"
#include "view/SpriteSelection.h"

namespace d2d
{

ArrangeSpriteFixOP::ArrangeSpriteFixOP(EditPanel* editPanel, 
									   MultiSpritesImpl* spritesImpl,
									   PropertySettingPanel* propertyPanel/* = NULL*/, 
									   ViewPanelMgr* view_panel_mgr/* = NULL*/,
									   AbstractEditCMPT* callback/* = NULL*/, 
									   const ArrangeSpriteConfig& cfg/* = NULL*/)
	: ArrangeSpriteOP(editPanel, spritesImpl, propertyPanel, NULL, callback, cfg)
	, m_spritesImpl(spritesImpl)
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
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		sprites[i]->Retain();

	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		m_spritesImpl->RemoveSprite(sprites[i]);
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		m_spritesImpl->InsertSprite(sprites[i]);

	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		sprites[i]->Release();
}

} // d2d