#include "SelectSpritesOP.h"
#include "StagePanel.h"

#include "frame/config.h"

#include <easyscale9.h>
#include <easymesh.h>
#include <easyanim.h>
#include <easycomplex.h>
#include <easytexture.h>
#include <easyshape.h>

namespace lr
{

SelectSpritesOP::SelectSpritesOP(d2d::EditPanel* editPanel, d2d::MultiSpritesImpl* spritesImpl, 
								 d2d::PropertySettingPanel* propertyPanel, d2d::AbstractEditCMPT* callback/* = NULL*/)
	: d2d::SelectSpritesOP(editPanel, spritesImpl, propertyPanel, callback)
{
	m_first_press.setInvalid();
}

bool SelectSpritesOP::onMouseLeftDown(int x, int y)
{
	if (d2d::SelectSpritesOP::onMouseLeftDown(x, y)) return true;

	if (!PATHFINDING) {
		return false;
	}

	d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
	if (m_first_press.isValid()) {
		StagePanel* stage = static_cast<StagePanel*>(m_editPanel);
		stage->Pathfinding(m_first_press, pos);
		m_first_press.setInvalid();
	} else {
		m_first_press = pos;
	}

	return false;
}

bool SelectSpritesOP::onMouseLeftDClick(int x, int y)
{
	if (d2d::SelectSpritesOP::onMouseLeftDClick(x, y)) return true;

	d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
	d2d::ISprite* selected = m_spritesImpl->querySpriteByPos(pos);
	if (ecomplex::Sprite* complex = dynamic_cast<ecomplex::Sprite*>(selected))
	{
 		ecomplex::Symbol& symbol = const_cast<ecomplex::Symbol&>(complex->getSymbol());
 		ecomplex::EditDialog dlg(m_editPanel, &symbol);
 		dlg.ShowModal();
 
  		//m_editPanel->resetCanvas();
		m_editPanel->ResetViewport();

		//////////////////////////////////////////////////////////////////////////

// 		std::string cmd = "easycomplex.exe " + complex->getSymbol().getFilepath();
// 		WinExec(cmd.c_str(), SW_SHOWMAXIMIZED);
	}
	else if (anim::Sprite* anim = dynamic_cast<anim::Sprite*>(selected))
	{
 		anim::PreviewDialog dlg(m_editPanel, &anim->getSymbol());
 		dlg.ShowModal();

// 		m_editPanel->resetCanvas();
		m_editPanel->ResetViewport();
	}
	else if (escale9::Sprite* patch9 = dynamic_cast<escale9::Sprite*>(selected))
 	{
		escale9::Symbol& symbol = const_cast<escale9::Symbol&>(patch9->getSymbol());
  		escale9::EditDialog dlg(m_editPanel, &symbol);
  		dlg.ShowModal();
  
// 		m_editPanel->resetCanvas();
		m_editPanel->ResetViewport();
 	}
	else if (emesh::Sprite* sprite = dynamic_cast<emesh::Sprite*>(selected))
	{
		emesh::EditDialog dlg(m_editPanel, sprite);
		dlg.ShowModal();

//		m_editPanel->resetCanvas();
		m_editPanel->ResetViewport();
	}
	else if (d2d::FontSprite* font = dynamic_cast<d2d::FontSprite*>(selected))
	{
		d2d::TextDialog dlg(m_editPanel, font);
		dlg.ShowModal();
	}
	else if (etexture::Sprite* tex = dynamic_cast<etexture::Sprite*>(selected))
	{
		etexture::Symbol& symbol = const_cast<etexture::Symbol&>(tex->getSymbol());
		etexture::EditDialog dlg(m_editPanel, &symbol);
		dlg.ShowModal();

		symbol.reloadTexture();
		m_editPanel->ResetViewport();
	}
	else if (libshape::Sprite* shape = dynamic_cast<libshape::Sprite*>(selected))
	{
//		libshape::Symbol& symbol = const_cast<libshape::Symbol&>(shape->getSymbol());

		std::vector<d2d::ISprite*> sprites;
		m_spritesImpl->traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));

		libshape::EditDialogSimple dlg(m_editPanel, shape, sprites);
//		libshape::EditDialog dlg(m_editPanel, &symbol);
		dlg.ShowModal();

		m_editPanel->ResetViewport();		
	}

	return false;
}


}