#include "SelectSpritesOP.h"

// for debug
//#include "MyThread.h"
#include "RectCutLoader.h"

namespace ecomplex
{

SelectSpritesOP::SelectSpritesOP(wxWindow* wnd, ee::EditPanelImpl* stage, ee::MultiSpritesImpl* spritesImpl, 
								 ee::EditCMPT* callback/* = NULL*/)
	: ee::SelectSpritesOP(wnd, stage, spritesImpl, callback)
	, m_open_symbol(wnd, stage, spritesImpl)
{
}

// bool SelectSpritesOP::OnKeyDown(int keyCode)
// {
// 	if (ee::SelectSpritesOP::OnKeyDown(keyCode)) {
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
	if (ee::SelectSpritesOP::OnMouseLeftDClick(x, y)) return true;

	ee::Vector pos = m_stage->TransPosScrToProj(x, y);
	ee::Sprite* selected = m_spritesImpl->QuerySpriteByPos(pos);
	if (selected) {
		m_open_symbol.Open(selected);
	}

	return false;
}

} // complex