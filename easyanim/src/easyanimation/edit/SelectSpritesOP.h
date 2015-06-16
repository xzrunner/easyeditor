#pragma once

#include <drag2d.h>

namespace eanim
{

class SelectSpritesOP : public d2d::SelectSpritesOP
{
public:
	SelectSpritesOP(d2d::EditPanel* editPanel, d2d::MultiSpritesImpl* spritesImpl, 
		d2d::PropertySettingPanel* propertyPanel, d2d::AbstractEditCMPT* callback = NULL);

	virtual bool OnMouseLeftDClick(int x, int y);

	virtual d2d::IPropertySetting* createPropertySetting(d2d::ISprite* sprite) const;

protected:
	virtual void PasteSprToClipboard(const d2d::ISprite* spr, Json::Value& value) const;
	virtual void CopySprFromClipboard(d2d::ISprite* spr, const Json::Value& value) const;

}; // SelectSpritesOP

}