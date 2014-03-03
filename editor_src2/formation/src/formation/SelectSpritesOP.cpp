#include "SelectSpritesOP.h"
#include "ActorPropertySetting.h"

using namespace formation;

SelectSpritesOP::SelectSpritesOP(d2d::EditPanel* editPanel, d2d::MultiSpritesImpl* spritesImpl, 
								 d2d::PropertySettingPanel* propertyPanel, d2d::AbstractEditCMPT* callback/* = NULL*/)
	: d2d::SelectSpritesOP(editPanel, spritesImpl, propertyPanel, callback)
{
}

d2d::IPropertySetting* SelectSpritesOP::createPropertySetting(d2d::ISprite* sprite) const
{
	if (!sprite) return NULL;

	std::vector<d2d::ISprite*> sprites;
	sprites.push_back(sprite);
	return new ActorPropertySetting(m_editPanel, sprites);
}

d2d::IPropertySetting* SelectSpritesOP::createPropertySetting(const std::vector<d2d::ISprite*>& sprites) const
{
	return new ActorPropertySetting(m_editPanel, sprites);
}
