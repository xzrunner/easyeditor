#include "SelectSpritesOP.h"
#include "ActorPropertySetting.h"

using namespace formation;

SelectSpritesOP::SelectSpritesOP(ee::EditPanel* editPanel, ee::MultiSpritesImpl* spritesImpl, 
								 ee::PropertySettingPanel* propertyPanel, ee::AbstractEditCMPT* callback/* = NULL*/)
	: ee::SelectSpritesOP(editPanel, spritesImpl, propertyPanel, callback)
{
}

ee::IPropertySetting* SelectSpritesOP::createPropertySetting(ee::ISprite* sprite) const
{
	if (!sprite) return NULL;

	std::vector<ee::ISprite*> sprites;
	sprites.push_back(sprite);
	return new ActorPropertySetting(m_editPanel, sprites);
}

ee::IPropertySetting* SelectSpritesOP::createPropertySetting(const std::vector<ee::ISprite*>& sprites) const
{
	return new ActorPropertySetting(m_editPanel, sprites);
}
