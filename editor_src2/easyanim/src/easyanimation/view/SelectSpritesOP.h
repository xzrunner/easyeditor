#pragma once

#include <drag2d.h>

namespace eanim
{
	class SelectSpritesOP : public d2d::SelectSpritesOP
	{
	public:
		SelectSpritesOP(d2d::EditPanel* editPanel, d2d::MultiSpritesImpl* spritesImpl, 
			d2d::PropertySettingPanel* propertyPanel, d2d::AbstractEditCMPT* callback = NULL,
			const d2d::Colorf& color = d2d::Colorf(0, 0, 0));

		virtual d2d::IPropertySetting* createPropertySetting(d2d::ISprite* sprite) const;

	}; // SelectSpritesOP
}