#include "SelectSpritesOP.h"
#include "Context.h"
#include "StagePanel.h"

#include <ee/SpriteSelection.h>
#include <ee/FetchAllVisitor.h>
#include <ee/EditPanelImpl.h>

#include <easycomplex.h>

namespace edb
{

SelectSpritesOP::SelectSpritesOP(wxWindow* stage_wnd, ee::EditPanelImpl* stage, 
								 ee::MultiSpritesImpl* sprites_impl, 
								 ee::EditCMPT* callback/* = NULL*/)
	: ee::SelectSpritesOP(stage_wnd, stage, sprites_impl, callback)
{
}

bool SelectSpritesOP::OnKeyDown(int keyCode)
{
	if (ee::SelectSpritesOP::OnKeyDown(keyCode)) return true;

	if (keyCode == WXK_SPACE)
	{
		ee::SpriteSelection* selection = Context::Instance()->stage->GetSpriteSelection();
		std::vector<ee::Sprite*> sprs;
		selection->Traverse(ee::FetchAllVisitor<ee::Sprite>(sprs));

		for (size_t i = 0, n = sprs.size(); i < n; ++i)
		{
			ecomplex::Sprite* complex = dynamic_cast<ecomplex::Sprite*>(sprs[i]);
			if (!complex) continue;

			ecomplex::Symbol* sym = dynamic_cast<ecomplex::Symbol*>(complex->GetSymbol());
			ecomplex::EditDialog dlg(m_stage->GetEditPanel(), sym, NULL);
			dlg.ShowModal();
		}
	}

	return false;
}

bool SelectSpritesOP::OnMouseLeftDClick(int x, int y)
{
	if (ee::SelectSpritesOP::OnMouseLeftDClick(x, y)) return true;

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	ee::Sprite* selected = m_spritesImpl->QuerySpriteByPos(pos);
	if (ecomplex::Sprite* complex = dynamic_cast<ecomplex::Sprite*>(selected))
	{
		ecomplex::Symbol* sym = dynamic_cast<ecomplex::Symbol*>(complex->GetSymbol());
		ecomplex::EditDialog dlg(m_stage->GetEditPanel(), sym, NULL);
		dlg.ShowModal();
	}
	//else if (eanim::Sprite* anim = dynamic_cast<eanim::Sprite*>(selected))
	//{
	//		eanim::PreviewDialog dlg(Context::Instance()->stage, Context::Instance()->library, &anim->getSymbol());
	//		dlg.ShowModal();
	//		Context::Instance()->stage->resetCanvas();
	//}
	// 	else if (ee::Scale9Sprite* patch9 = dynamic_cast<ee::Scale9Sprite*>(selected))
	// 	{
	//  		ee::Scale9Symbol& sym = const_cast<ee::Scale9Symbol&>(patch9->getSymbol());
	//  		escale9::EditDialog dlg(m_stage, NULL, &sym);
	//  		dlg.ShowModal();
	//  
	////   		std::vector<ee::Scale9Sprite*> sprs;
	////   		m_stage->traverseSprites(ee::FetchAllVisitor<ee::Scale9Sprite>(sprs));
	////   		for (size_t i = 0, n = sprs.size(); i < n; ++i)
	////   		{
	////   			ee::Sprite* spr = sprs[i];
	////   			if (&spr->getSymbol() == &sym)
	////   				spr->buildBounding();
	////   		}
	//  
	// 		m_stage->resetCanvas();
	// 	}

	return false;
}

}