#include "SelectSpritesOP.h"
#include "Context.h"
#include "StagePanel.h"

#include <easy9patch.h>

namespace ecomplex
{

SelectSpritesOP::SelectSpritesOP(d2d::EditPanel* editPanel, d2d::MultiSpritesImpl* spritesImpl, 
								 d2d::PropertySettingPanel* propertyPanel, d2d::AbstractEditCMPT* callback/* = NULL*/,
								 const d2d::Colorf& color/* = d2d::Colorf(0, 0, 0)*/)
								 : libcomplex::SelectSpritesOP(editPanel, spritesImpl, propertyPanel, callback, color)
{
}

d2d::IPropertySetting* 
SelectSpritesOP::createPropertySetting(d2d::ISprite* sprite) const
{
	if (sprite)
		return libcomplex::SelectSpritesOP::createPropertySetting(sprite);
	else
		return new d2d::SymbolPropertySetting(m_editPanel, Context::Instance()->stage->getSymbol());
}

} // ecomplex