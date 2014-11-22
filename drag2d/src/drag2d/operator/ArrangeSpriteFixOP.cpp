#include "ArrangeSpriteFixOP.h"

#include "view/MultiSpritesImpl.h"

namespace d2d
{

ArrangeSpriteFixOP::ArrangeSpriteFixOP(EditPanel* editPanel, 
									   MultiSpritesImpl* spritesImpl,
									   PropertySettingPanel* propertyPanel/* = NULL*/, 
									   AbstractEditCMPT* callback/* = NULL*/, 
									   const ArrangeSpriteConfig& cfg)
	: ArrangeSpriteOP(editPanel, spritesImpl, propertyPanel, callback, cfg)
	, m_spritesImpl(spritesImpl)
{
}

bool ArrangeSpriteFixOP::onMouseLeftUp(int x, int y)
{
	if (ArrangeSpriteOP::onMouseLeftUp(x, y)) return true;
	FixSpritesLocation();
	return false;
}

// bool ArrangeSpriteFixOP::onMouseRightDown(int x, int y)
// {
// 	if (ArrangeSpriteOP::onMouseRightDown(x, y)) return true;
// 
// 	onMouseLeftDown(x, y);
// 
// 	return false;
// }

bool ArrangeSpriteFixOP::onMouseRightUp(int x, int y)
{
	if (ArrangeSpriteOP::onMouseRightUp(x, y)) return true;
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
		sprites[i]->retain();

	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		m_spritesImpl->removeSprite(sprites[i]);
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		m_spritesImpl->insertSprite(sprites[i]);

	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		sprites[i]->release();
}

} // d2d