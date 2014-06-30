#include "SelectSpritesOP.h"

#include "frame/Context.h"

#include <easymesh.h>

namespace eanim
{

SelectSpritesOP::SelectSpritesOP(d2d::EditPanel* editPanel, d2d::MultiSpritesImpl* spritesImpl, 
	d2d::PropertySettingPanel* propertyPanel, d2d::AbstractEditCMPT* callback/* = NULL*/)
		: d2d::SelectSpritesOP(editPanel, spritesImpl, propertyPanel, callback)
{
}

bool SelectSpritesOP::onMouseLeftDClick(int x, int y)
{
	if (d2d::SelectSpritesOP::onMouseLeftDClick(x, y)) 
		return true;

	d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
	d2d::ISprite* selected = m_spritesImpl->querySpriteByPos(pos);
	if (emesh::Sprite* sprite = dynamic_cast<emesh::Sprite*>(selected))
	{
		emesh::EditDialog dlg(m_editPanel, sprite);
		dlg.ShowModal();

		m_editPanel->resetCanvas();
	}

	return false;
}

d2d::IPropertySetting* SelectSpritesOP::createPropertySetting(d2d::ISprite* sprite) const
{
	if (sprite)
		return d2d::SelectSpritesOP::createPropertySetting(sprite);
	else
		return new d2d::SymbolPropertySetting(m_editPanel, &Context::Instance()->name);
}

}