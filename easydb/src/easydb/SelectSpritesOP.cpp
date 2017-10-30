#include "SelectSpritesOP.h"
#include "StagePanel.h"
#include "Symbol.h"
#include "Sprite.h"

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
		ee::SpriteSelection* selection = static_cast<StagePanel*>(m_wnd)->GetSpriteSelection();
		std::vector<ee::SprPtr> sprs;
		selection->Traverse(ee::FetchAllRefVisitor<ee::Sprite>(sprs));

		for (size_t i = 0, n = sprs.size(); i < n; ++i)
		{
			ecomplex::Sprite* complex = std::dynamic_pointer_cast<ecomplex::Sprite>(sprs[i]);
			if (!complex) continue;

			ecomplex::Symbol* sym = dynamic_cast<ecomplex::Symbol*>(complex->GetSymbol());
			ecomplex::EditDialog dlg(m_stage->GetEditPanel(), sym, NULL);
			dlg.ShowModal();
		}
	}

	return false;
}

bool SelectSpritesOP::OnMouseLeftDown(int x, int y)
{
	bool ret = ee::SelectSpritesOP::OnMouseLeftDown(x, y);

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	if (m_selection->Size() != 1) {
		return ret;
	}

	std::vector<ee::SprPtr> selection;
	m_selection->Traverse(ee::FetchAllRefVisitor<ee::Sprite>(selection));
	Symbol::OnSprPressed(dynamic_cast<Sprite*>(selection[0]), pos);

	return ret;
}

bool SelectSpritesOP::OnMouseLeftDClick(int x, int y)
{
	if (ee::SelectSpritesOP::OnMouseLeftDClick(x, y)) return true;

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	auto selected = m_sprs_impl->QuerySpriteByPos(pos);
	if (ecomplex::Sprite* complex = std::dynamic_pointer_cast<ecomplex::Sprite>(selected))
	{
		ecomplex::Symbol* sym = dynamic_cast<ecomplex::Symbol*>(complex->GetSymbol());
		ecomplex::EditDialog dlg(m_stage->GetEditPanel(), sym, NULL);
		dlg.ShowModal();
	}
	//else if (libanim::Sprite* anim = std::dynamic_pointer_cast<libanim::Sprite>(selected))
	//{
	//		libanim::PreviewDialog dlg(Context::Instance()->stage, Context::Instance()->library, &anim->getSymbol());
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
	////   			auto& spr = sprs[i];
	////   			if (&spr->getSymbol() == &sym)
	////   				spr->buildBounding();
	////   		}
	//  
	// 		m_stage->resetCanvas();
	// 	}

	return false;
}

}