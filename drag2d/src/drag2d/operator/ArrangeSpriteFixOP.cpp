#include "ArrangeSpriteFixOP.h"

#include "view/MultiSpritesImpl.h"

namespace d2d
{

ArrangeSpriteFixOP::ArrangeSpriteFixOP(EditPanel* editPanel, MultiSpritesImpl* spritesImpl,
									   PropertySettingPanel* propertyPanel/* = NULL*/, 
									   AbstractEditCMPT* callback/* = NULL*/, bool isDeformOpen /*= true*/)
	: ArrangeSpriteOP(editPanel, spritesImpl, propertyPanel, callback, isDeformOpen)
	, m_spritesImpl(spritesImpl)
{
}

bool ArrangeSpriteFixOP::onMouseLeftUp(int x, int y)
{
	if (ArrangeSpriteOP::onMouseLeftUp(x, y)) return true;

	std::vector<ISprite*> sprites;
	m_selection->traverse(FetchAllVisitor<ISprite>(sprites));
	fixSpritesLocation(sprites);

	return false;
}

bool ArrangeSpriteFixOP::onMouseRightDown(int x, int y)
{
	if (ArrangeSpriteOP::onMouseRightDown(x, y)) return true;

	onMouseLeftDown(x, y);

	return false;
}

bool ArrangeSpriteFixOP::onMouseRightUp(int x, int y)
{
	return onMouseLeftUp(x, y);
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