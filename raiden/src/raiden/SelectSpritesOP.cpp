#include "SelectSpritesOP.h"
#include "ActorPropertySetting.h"

using namespace raiden;

SelectSpritesOP::SelectSpritesOP(d2d::EditPanel* editPanel, d2d::MultiSpritesImpl* spritesImpl, 
								 d2d::PropertySettingPanel* propertyPanel, d2d::AbstractEditCMPT* callback/* = NULL*/)
	: d2d::SelectSpritesOP(editPanel, spritesImpl, propertyPanel, callback)
{
}

bool SelectSpritesOP::onMouseLeftDClick(int x, int y)
{
	if (d2d::SelectSpritesOP::onMouseLeftDClick(x, y)) return true;

	d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
	d2d::ISprite* selected = m_spritesImpl->querySpriteByPos(pos);
	if (selected)
	{
//		libshape::MainDialog<BodyEditStage> dlg(Context::Instance()->stage, selected);
//		dlg.ShowModal();
//		Context::Instance()->stage->resetCanvas();
	}

	return false;
}

d2d::IPropertySetting* SelectSpritesOP::createPropertySetting(d2d::ISprite* sprite) const
{
	if (!sprite) return NULL;

	std::vector<d2d::ISprite*> sprites;
	sprites.push_back(sprite);
	return new ActorPropertySetting(m_editPanel, sprites);
}

d2d::IPropertySetting* SelectSpritesOP::createPropertySetting(const std::vector<d2d::ISprite*>& sprites) const
{
	return new ActorPropertySetting(m_editPanel, sprites);
}
