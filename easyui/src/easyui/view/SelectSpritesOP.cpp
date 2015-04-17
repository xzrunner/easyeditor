#include "SelectSpritesOP.h"
#include "StagePanel.h"
#include "FontPropertySetting.h"

namespace eui
{

SelectSpritesOP::SelectSpritesOP(d2d::EditPanel* editPanel, d2d::MultiSpritesImpl* spritesImpl, 
								 d2d::PropertySettingPanel* property, d2d::AbstractEditCMPT* callback)
	: d2d::SelectSpritesOP(editPanel, spritesImpl, property)
{
}

d2d::IPropertySetting* SelectSpritesOP::createPropertySetting(d2d::ISprite* sprite) const
{
	if (d2d::TextSprite* text = dynamic_cast<d2d::TextSprite*>(sprite))
		return new d2d::TextPropertySetting(m_editPanel, text);
	else if (d2d::FontSprite* font = dynamic_cast<d2d::FontSprite*>(sprite))
		return new FontPropertySetting(m_editPanel, font);
	else if (sprite)
		return new d2d::SpritePropertySetting(m_editPanel, sprite);
	else 
		return NULL;	
}

}