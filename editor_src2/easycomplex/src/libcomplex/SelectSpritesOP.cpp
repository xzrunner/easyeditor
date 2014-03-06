#include "SelectSpritesOP.h"
#include "EditDialog.h"
#include "StagePanel.h"
#include "Sprite.h"
#include "PropertySetting.h"

// #include <easyanim.h>
#include <easyscale9.h>
#include <easymesh.h>

namespace ecomplex
{

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
	if (ecomplex::Sprite* complex = dynamic_cast<ecomplex::Sprite*>(selected))
	{
// 		Symbol& symbol = const_cast<Symbol&>(complex->getSymbol());
// 		EditDialog dlg(m_editPanel, &symbol);
// 		dlg.ShowModal();
// 
//  		m_editPanel->resetCanvas();

		//////////////////////////////////////////////////////////////////////////

		std::string cmd = "easycomplex.exe " + complex->getSymbol().getFilepath();
		WinExec(cmd.c_str(), SW_SHOWMAXIMIZED);
	}
	//else if (anim::Sprite* anim = dynamic_cast<anim::Sprite*>(selected))
	//{
 //		anim::PreviewDialog dlg(Context::Instance()->stage, Context::Instance()->library, &anim->getSymbol());
 //		dlg.ShowModal();
 //		Context::Instance()->stage->resetCanvas();
	//}
 	else if (d2d::Scale9Sprite* patch9 = dynamic_cast<d2d::Scale9Sprite*>(selected))
 	{
  		d2d::Scale9Symbol& symbol = const_cast<d2d::Scale9Symbol&>(patch9->getSymbol());
  		libscale9::EditDialog dlg(m_editPanel, &symbol);
  		dlg.ShowModal();
  
 		m_editPanel->resetCanvas();
 	}
	else if (emesh::Sprite* sprite = dynamic_cast<emesh::Sprite*>(selected))
	{
		emesh::Symbol& symbol = const_cast<emesh::Symbol&>(sprite->getSymbol());
		emesh::EditDialog dlg(m_editPanel, &symbol);
		dlg.ShowModal();

		m_editPanel->resetCanvas();
	}

	return false;
}

d2d::IPropertySetting* 
SelectSpritesOP::createPropertySetting(d2d::ISprite* sprite) const
{
	if (sprite)
		return d2d::SelectSpritesOP::createPropertySetting(sprite);
	else
		return new ecomplex::PropertySetting(m_editPanel, static_cast<StagePanel*>(m_editPanel)->getSymbol());
}

} // complex