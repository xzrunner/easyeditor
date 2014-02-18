#include "SelectSpritesOP.h"
#include "EditDialog.h"
#include "StagePanel.h"
#include "Sprite.h"
#include "PropertySetting.h"

// #include <easyanim.h>
#include <easy9patch.h>

namespace complex
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
	if (complex::Sprite* complex = dynamic_cast<complex::Sprite*>(selected))
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
 	else if (d2d::Patch9Sprite* patch9 = dynamic_cast<d2d::Patch9Sprite*>(selected))
 	{
  		d2d::Patch9Symbol& symbol = const_cast<d2d::Patch9Symbol&>(patch9->getSymbol());
  		lib9patch::EditDialog dlg(m_editPanel, &symbol);
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
		return new complex::PropertySetting(m_editPanel, static_cast<StagePanel*>(m_editPanel)->getSymbol());
}

} // complex