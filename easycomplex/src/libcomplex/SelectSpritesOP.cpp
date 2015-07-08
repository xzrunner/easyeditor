#include "SelectSpritesOP.h"
#include "EditDialog.h"
#include "StagePanel.h"
#include "Sprite.h"

#include <easyscale9.h>
#include <easymesh.h>
#include <easyscale9.h>
#include <easyanim.h>
#include <easytexture.h>
#include <easyicon.h>
#include <easyshadow.h>

// for debug
//#include "MyThread.h"
#include "RectCutLoader.h"

namespace ecomplex
{

SelectSpritesOP::SelectSpritesOP(d2d::EditPanel* editPanel, d2d::MultiSpritesImpl* spritesImpl, 
								 d2d::ViewPanelMgr* view_panel_mgr, d2d::AbstractEditCMPT* callback/* = NULL*/)
	: d2d::SelectSpritesOP(editPanel, spritesImpl, view_panel_mgr, callback)
{
}

// bool SelectSpritesOP::OnKeyDown(int keyCode)
// {
// 	if (d2d::SelectSpritesOP::OnKeyDown(keyCode)) {
// 		return true;
// 	}
// 
// 	//  	if (keyCode == WXK_ESCAPE)
// 	//  	{
// 	//  		MyThread* t = new MyThread();
// 	//  		t->Create();
// 	//  		t->Run();
// 	//  	}
// 
//  	if (keyCode == WXK_SPACE)
//  	{
//  		RectCutLoader loader((StagePanel*)(m_stage));
// // 		loader.LoadOnlyJson("E:\\debug\\character\\debug_pack\\pack.json", "2013baji1_attack1_2_1_011");
// 		loader.LoadJsonAndImg("E:\\debug\\character\\pack\\pack.json", "shadow");
// // 		loader.LoadToDtex("E:\\debug\\character\\debug_pack\\pack.json", "2013baji1_attack1_2_1_011");
// //		loader.LoadToDtex("E:\\debug\\character\\pack\\pack.rrp", "2013baji1_attack1_2_1_011");
//  	}
// 
// 	return false;
// }

bool SelectSpritesOP::OnMouseLeftDClick(int x, int y)
{
	if (d2d::SelectSpritesOP::OnMouseLeftDClick(x, y)) return true;

	d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
	d2d::ISprite* selected = m_spritesImpl->QuerySpriteByPos(pos);
	if (ecomplex::Sprite* complex = dynamic_cast<ecomplex::Sprite*>(selected))
	{
 		Symbol& symbol = const_cast<Symbol&>(complex->getSymbol());
 		EditDialog dlg(m_stage, &symbol);
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
		m_spritesImpl->TraverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));

		etexture::EditDialog dlg(m_stage, tex, sprites);
		dlg.ShowModal();

		m_stage->ResetViewport();
	}
	else if (eshadow::Sprite* shadow = dynamic_cast<eshadow::Sprite*>(selected))
	{
		
	}

	return false;
}

} // complex