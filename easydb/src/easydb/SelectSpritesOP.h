#pragma once

#include <drag2d.h>

namespace edb
{
	class SelectSpritesOP : public d2d::SelectSpritesOP
	{
	public:
		SelectSpritesOP(d2d::EditPanel* editPanel, d2d::MultiSpritesImpl* spritesImpl, 
			d2d::PropertySettingPanel* propertyPanel, d2d::AbstractEditCMPT* callback = NULL);

		virtual bool onKeyDown(int keyCode); 
		virtual bool onMouseLeftDClick(int x, int y);

	}; // SelectSpritesOP
}