#include "SelectSpritesOP.h"

#include "frame/Context.h"

namespace eanim
{
	SelectSpritesOP::SelectSpritesOP(d2d::EditPanel* editPanel, d2d::MultiSpritesImpl* spritesImpl, 
		d2d::PropertySettingPanel* propertyPanel, d2d::AbstractEditCMPT* callback/* = NULL*/,
		const d2d::Colorf& color/* = d2d::Colorf(0, 0, 0)*/)
			: d2d::SelectSpritesOP(editPanel, spritesImpl, propertyPanel, callback, color)
	{
	}

	d2d::IPropertySetting* SelectSpritesOP::createPropertySetting(d2d::ISprite* sprite) const
	{
		if (sprite)
			return d2d::SelectSpritesOP::createPropertySetting(sprite);
		else
			return new d2d::SymbolPropertySetting(m_editPanel, &Context::Instance()->name);
	}
}