#ifndef FORMATION_SELECT_SPRITES_OP_H
#define FORMATION_SELECT_SPRITES_OP_H

#include <drag2d.h>

namespace formation
{
	class SelectSpritesOP : public d2d::SelectSpritesOP
	{
	public:
		SelectSpritesOP(d2d::EditPanel* editPanel, d2d::MultiSpritesImpl* spritesImpl, 
			d2d::PropertySettingPanel* propertyPanel, d2d::AbstractEditCMPT* callback = NULL);

		virtual d2d::IPropertySetting* createPropertySetting(d2d::ISprite* sprite) const;
		virtual d2d::IPropertySetting* createPropertySetting(const std::vector<d2d::ISprite*>& sprites) const;

	}; // SelectSpritesOP
}

#endif // FORMATION_SELECT_SPRITES_OP_H