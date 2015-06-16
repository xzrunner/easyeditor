#include "SelectSpritesOP.h"

#include <easycomplex.h>

#include "Context.h"
#include "StagePanel.h"

namespace edb
{
	SelectSpritesOP::SelectSpritesOP(d2d::EditPanel* editPanel, d2d::MultiSpritesImpl* spritesImpl, 
		d2d::PropertySettingPanel* propertyPanel, d2d::AbstractEditCMPT* callback/* = NULL*/)
		: d2d::SelectSpritesOP(editPanel, spritesImpl, propertyPanel, NULL, callback)
	{
	}

	bool SelectSpritesOP::OnKeyDown(int keyCode)
	{
		if (d2d::SelectSpritesOP::OnKeyDown(keyCode)) return true;

		if (keyCode == WXK_SPACE)
		{
			d2d::SpriteSelection* selection = Context::Instance()->stage->getSpriteSelection();
			std::vector<d2d::ISprite*> sprites;
			selection->Traverse(d2d::FetchAllVisitor<d2d::ISprite>(sprites));

			for (size_t i = 0, n = sprites.size(); i < n; ++i)
			{
				ecomplex::Sprite* complex = dynamic_cast<ecomplex::Sprite*>(sprites[i]);
				if (!complex) continue;

				ecomplex::Symbol& symbol = const_cast<ecomplex::Symbol&>(complex->getSymbol());
				ecomplex::EditDialog dlg(m_stage, &symbol);
				dlg.ShowModal();

				m_stage->resetCanvas();
			}
		}

		return false;
	}

	bool SelectSpritesOP::OnMouseLeftDClick(int x, int y)
	{
		if (d2d::SelectSpritesOP::OnMouseLeftDClick(x, y)) return true;

		d2d::Vector pos = m_stage->transPosScreenToProject(x, y);
		d2d::ISprite* selected = m_spritesImpl->querySpriteByPos(pos);
		if (ecomplex::Sprite* complex = dynamic_cast<ecomplex::Sprite*>(selected))
		{
			ecomplex::Symbol& symbol = const_cast<ecomplex::Symbol&>(complex->getSymbol());
			ecomplex::EditDialog dlg(m_stage, &symbol);
			dlg.ShowModal();

			m_stage->resetCanvas();
		}
		//else if (anim::Sprite* anim = dynamic_cast<anim::Sprite*>(selected))
		//{
		//		anim::PreviewDialog dlg(Context::Instance()->stage, Context::Instance()->library, &anim->getSymbol());
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
		////   			d2d::ISprite* sprite = sprites[i];
		////   			if (&sprite->getSymbol() == &symbol)
		////   				sprite->buildBounding();
		////   		}
		//  
		// 		m_stage->resetCanvas();
		// 	}

		return false;
	}
}