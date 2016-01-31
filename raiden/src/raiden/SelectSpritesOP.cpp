#include "SelectSpritesOP.h"
#include "ActorPropertySetting.h"

using namespace raiden;

SelectSpritesOP::SelectSpritesOP(ee::EditPanel* editPanel, ee::MultiSpritesImpl* spritesImpl, 
								 ee::PropertySettingPanel* propertyPanel, ee::AbstractEditCMPT* callback/* = NULL*/)
	: ee::SelectSpritesOP(editPanel, spritesImpl, propertyPanel, callback)
{
}

bool SelectSpritesOP::onMouseLeftDClick(int x, int y)
{
	if (ee::SelectSpritesOP::onMouseLeftDClick(x, y)) return true;

	ee::Vector pos = m_editPanel->transPosScreenToProject(x, y);
	ee::ISprite* selected = m_spritesImpl->querySpriteByPos(pos);
	if (selected)
	{
//		libshape::MainDialog<BodyEditStage> dlg(Context::Instance()->stage, selected);
//		dlg.ShowModal();
//		Context::Instance()->stage->resetCanvas();
	}

	return false;
}

ee::IPropertySetting* SelectSpritesOP::createPropertySetting(ee::ISprite* sprite) const
{
	if (!sprite) return NULL;

	std::vector<ee::ISprite*> sprites;
	sprites.push_back(sprite);
	return new ActorPropertySetting(m_editPanel, sprites);
}

ee::IPropertySetting* SelectSpritesOP::createPropertySetting(const std::vector<ee::ISprite*>& sprites) const
{
	return new ActorPropertySetting(m_editPanel, sprites);
}
