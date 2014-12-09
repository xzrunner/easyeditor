#pragma once

#include <drag2d.h>

namespace ecomplex
{
	class SelectSpritesOP : public d2d::SelectSpritesOP
	{
	public:
		SelectSpritesOP(d2d::EditPanel* editPanel, d2d::MultiSpritesImpl* spritesImpl, 
			d2d::PropertySettingPanel* propertyPanel, d2d::AbstractEditCMPT* callback = NULL);

		virtual bool onKeyDown(int keyCode);
		virtual bool onMouseLeftDClick(int x, int y);

		virtual d2d::IPropertySetting* createPropertySetting(d2d::ISprite* sprite) const;

	}; // SelectSpritesOP
}

