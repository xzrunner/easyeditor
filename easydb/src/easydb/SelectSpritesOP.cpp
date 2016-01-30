#include "SelectSpritesOP.h"
#include "Context.h"
#include "StagePanel.h"

#include <easycomplex.h>

namespace edb
{

SelectSpritesOP::SelectSpritesOP(wxWindow* stage_wnd, d2d::EditPanelImpl* stage, 
								 d2d::MultiSpritesImpl* spritesImpl, 
								 d2d::AbstractEditCMPT* callback/* = NULL*/)
	: d2d::SelectSpritesOP(stage_wnd, stage, spritesImpl, NULL, callback)
{
}

bool SelectSpritesOP::OnKeyDown(int keyCode)
{
	if (d2d::SelectSpritesOP::OnKeyDown(keyCode)) return true;

	if (keyCode == WXK_SPACE)
	{
		d2d::SpriteSelection* selection = Context::Instance()->stage->GetSpriteSelection();
		std::vector<d2d::Sprite*> sprites;
		selection->Traverse(d2d::FetchAllVisitor<d2d::Sprite>(sprites));

		for (size_t i = 0, n = sprites.size(); i < n; ++i)
		{
			ecomplex::Sprite* complex = dynamic_cast<ecomplex::Sprite*>(sprites[i]);
			if (!complex) continue;

			ecomplex::Symbol& symbol = const_cast<ecomplex::Symbol&>(complex->GetSymbol());
			ecomplex::EditDialog dlg(m_stage, &symbol);
			dlg.ShowModal();
		}
	}

	return false;
}

bool SelectSpritesOP::OnMouseLeftDClick(int x, int y)
{
	if (d2d::SelectSpritesOP::OnMouseLeftDClick(x, y)) return true;

	d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
	d2d::Sprite* selected = m_spritesImpl->QuerySpriteByPos(pos);
	if (ecomplex::Sprite* complex = dynamic_cast<ecomplex::Sprite*>(selected))
	{
		ecomplex::Symbol& symbol = const_cast<ecomplex::Symbol&>(complex->GetSymbol());
		ecomplex::EditDialog dlg(m_stage, &symbol);
		dlg.ShowModal();
	}
	//else if (libanim::Sprite* anim = dynamic_cast<libanim::Sprite*>(selected))
	//{
	//		libanim::PreviewDialog dlg(Context::Instance()->stage, Context::Instance()->library, &anim->getSymbol());
	//		dlg.ShowModal();
	//		Context::Instance()->stage->resetCanvas();
	//}
	// 	else if (d2d::Scale9Sprite* patch9 = dynamic_cast<d2d::Scale9Sprite*>(selected))
	// 	{
	//  		d2d::Scale9Symbol& symbol = const_cast<d2d::Scale9Symbol&>(patch9->getSymbol());
	//  		escale9::EditDialog dlg(m_stage, NULL, &symbol);
	//  		dlg.ShowModal();
	//  
	////   		std::vector<d2d::Scale9Sprite*> sprites;
	////   		m_stage->traverseSprites(d2d::FetchAllVisitor<d2d::Scale9Sprite>(sprites));
	////   		for (size_t i = 0, n = sprites.size(); i < n; ++i)
	////   		{
	////   			d2d::Sprite* sprite = sprites[i];
	////   			if (&sprite->getSymbol() == &symbol)
	////   				sprite->buildBounding();
	////   		}
	//  
	// 		m_stage->resetCanvas();
	// 	}

	return false;
}

}