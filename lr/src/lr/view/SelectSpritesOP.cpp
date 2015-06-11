#include "SelectSpritesOP.h"
#include "StagePanel.h"

#include "frame/config.h"

#include <easyscale9.h>
#include <easymesh.h>
#include <easyanim.h>
#include <easycomplex.h>
#include <easytexture.h>
#include <easyshape.h>
#include <easyterrain2d.h>

namespace lr
{

SelectSpritesOP::SelectSpritesOP(d2d::EditPanel* editPanel, d2d::MultiSpritesImpl* spritesImpl, 
								 d2d::PropertySettingPanel* propertyPanel, d2d::AbstractEditCMPT* callback/* = NULL*/)
	: d2d::SelectSpritesOP(editPanel, spritesImpl, propertyPanel, NULL, callback)
{
	m_first_press.setInvalid();
}

bool SelectSpritesOP::onMouseLeftDown(int x, int y)
{
	if (d2d::SelectSpritesOP::onMouseLeftDown(x, y)) return true;

	d2d::Vector pos = m_stage->transPosScreenToProject(x, y);

	StagePanel* stage = static_cast<StagePanel*>(m_stage);
	stage->PointQuery(pos);

	if (m_first_press.isValid()) {
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

	d2d::Vector pos = m_stage->transPosScreenToProject(x, y);
	d2d::ISprite* selected = m_spritesImpl->querySpriteByPos(pos);
	if (ecomplex::Sprite* complex = dynamic_cast<ecomplex::Sprite*>(selected))
	{
 		ecomplex::Symbol& symbol = const_cast<ecomplex::Symbol&>(complex->getSymbol());
 		ecomplex::EditDialog dlg(m_stage, &symbol);
 		dlg.ShowModal();
 
  		//m_stage->resetCanvas();
		m_stage->ResetViewport();

		//////////////////////////////////////////////////////////////////////////

// 		std::string cmd = "easycomplex.exe " + complex->getSymbol().getFilepath();
// 		WinExec(cmd.c_str(), SW_SHOWMAXIMIZED);
	}
	else if (libanim::Sprite* anim = dynamic_cast<libanim::Sprite*>(selected))
	{
 		libanim::PreviewDialog dlg(m_stage, &anim->getSymbol());
 		dlg.ShowModal();

// 		m_stage->resetCanvas();
		m_stage->ResetViewport();
	}
	else if (escale9::Sprite* patch9 = dynamic_cast<escale9::Sprite*>(selected))
 	{
		escale9::Symbol& symbol = const_cast<escale9::Symbol&>(patch9->getSymbol());
  		escale9::EditDialog dlg(m_stage, &symbol);
  		dlg.ShowModal();
  
// 		m_stage->resetCanvas();
		m_stage->ResetViewport();
 	}
	else if (emesh::Sprite* sprite = dynamic_cast<emesh::Sprite*>(selected))
	{
		emesh::EditDialog dlg(m_stage, sprite);
		dlg.ShowModal();

//		m_stage->resetCanvas();
		m_stage->ResetViewport();
	}
	else if (d2d::FontSprite* font = dynamic_cast<d2d::FontSprite*>(selected))
	{
		d2d::TextDialog dlg(m_stage, font);
		dlg.ShowModal();
	}
	else if (etexture::Sprite* tex = dynamic_cast<etexture::Sprite*>(selected))
	{
		std::vector<d2d::ISprite*> sprites;
		m_spritesImpl->traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));

		etexture::EditDialog dlg(m_stage, tex, sprites);
		dlg.ShowModal();

		m_stage->ResetViewport();
	}
	else if (libshape::Sprite* shape = dynamic_cast<libshape::Sprite*>(selected))
	{
//		libshape::Symbol& symbol = const_cast<libshape::Symbol&>(shape->getSymbol());

		std::vector<d2d::ISprite*> sprites;
		m_spritesImpl->traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));

		libshape::EditDialogSimple dlg(m_stage, shape, sprites);
//		libshape::EditDialog dlg(m_stage, &symbol);
		dlg.ShowModal();

		m_stage->ResetViewport();		
	}
	else if (eterrain2d::Sprite* terr = dynamic_cast<eterrain2d::Sprite*>(selected))
	{
		std::vector<d2d::ISprite*> sprites;
		m_spritesImpl->traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));

		eterrain2d::EditDialog dlg(m_stage, terr, sprites);
		dlg.ShowModal();

		m_stage->ResetViewport();
	}

	return false;
}


}