#include "SelectSpritesOP.h"
#include "EditDialog.h"
#include "StagePanel.h"
#include "Sprite.h"
#include "PropertySetting.h"

#include <easyscale9.h>
#include <easymesh.h>
#include <easyscale9.h>
#include <easyanim.h>

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
 		Symbol& symbol = const_cast<Symbol&>(complex->getSymbol());
 		EditDialog dlg(m_editPanel, &symbol);
 		dlg.ShowModal();
 
  		m_editPanel->resetCanvas();

		//////////////////////////////////////////////////////////////////////////

// 		std::string cmd = "easycomplex.exe " + complex->getSymbol().getFilepath();
// 		WinExec(cmd.c_str(), SW_SHOWMAXIMIZED);
	}
	else if (anim::Sprite* anim = dynamic_cast<anim::Sprite*>(selected))
	{
 		anim::PreviewDialog dlg(m_editPanel, &anim->getSymbol());
 		dlg.ShowModal();

 		m_editPanel->resetCanvas();
	}
	else if (escale9::Sprite* patch9 = dynamic_cast<escale9::Sprite*>(selected))
 	{
		escale9::Symbol& symbol = const_cast<escale9::Symbol&>(patch9->getSymbol());
  		escale9::EditDialog dlg(m_editPanel, &symbol);
  		dlg.ShowModal();
  
 		m_editPanel->resetCanvas();
 	}
	else if (emesh::Sprite* sprite = dynamic_cast<emesh::Sprite*>(selected))
	{
		emesh::EditDialog dlg(m_editPanel, sprite);
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