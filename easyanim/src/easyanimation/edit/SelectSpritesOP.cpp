#include "SelectSpritesOP.h"

#include "dataset/SpriteUserData.h"
#include "frame/Controller.h"
#include "view/StagePanel.h"

#include <easymesh.h>
#include <easyicon.h>

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
	if (sprite) {
		if (eicon::Sprite* spr = dynamic_cast<eicon::Sprite*>(sprite)) {
			return new eicon::SpritePropertySetting(m_editPanel, spr);
		} else {
			return d2d::SelectSpritesOP::createPropertySetting(sprite);
		}
	} else {
		Controller* ctrl = ((StagePanel*)m_editPanel)->GetController();
		return new d2d::SymbolPropertySetting(m_editPanel, &ctrl->name);
	}
}

void SelectSpritesOP::PasteSprToClipboard(const d2d::ISprite* spr, Json::Value& value) const
{
	d2d::SelectSpritesOP::PasteSprToClipboard(spr, value);

	SpriteUserData* ud = (SpriteUserData*)spr->getUserData();
	value["layer"] = ud->layer;
	value["frame"] = ud->frame;
}

void SelectSpritesOP::CopySprFromClipboard(d2d::ISprite* spr, const Json::Value& value) const
{
	d2d::SelectSpritesOP::CopySprFromClipboard(spr, value);

	set_sprite_user_data(spr, value["layer"].asInt(), value["frame"].asInt());
}

}