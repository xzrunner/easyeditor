#include "SelectSpritesOP.h"
#include "StagePanel.h"
#include "LibraryPanel.h"
#include "Context.h"

#include "complex/Dialog.h"

#include <easyanim.h>
#include <easyscale9.h>

namespace coceditor
{

SelectSpritesOP::SelectSpritesOP(ee::EditPanel* editPanel, ee::MultiSpritesImpl* spritesImpl, 
								 ee::PropertySettingPanel* propertyPanel, ee::AbstractEditCMPT* callback/* = NULL*/,
								 const ee::Colorf& color/* = ee::Colorf(0, 0, 0)*/)
	: ee::SelectSpritesOP(editPanel, spritesImpl, propertyPanel, callback, color)
{
}

bool SelectSpritesOP::onMouseLeftDClick(int x, int y)
{
	if (ee::SelectSpritesOP::onMouseLeftDClick(x, y)) return true;

	ee::Vector pos = m_editPanel->transPosScreenToProject(x, y);
	ee::ISprite* selected = m_spritesImpl->querySpriteByPos(pos);
	if (complex::Sprite* complex = dynamic_cast<complex::Sprite*>(selected))
	{
		complex::Dialog dlg(Context::Instance()->stage, 
			const_cast<complex::Symbol*>(&complex->getSymbol()), Context::Instance()->property);
		dlg.ShowModal();
		Context::Instance()->stage->resetCanvas();
	}
	else if (ee::AnimSprite* anim = dynamic_cast<ee::AnimSprite*>(selected))
	{
		libanim::PreviewDialog dlg(Context::Instance()->stage, Context::Instance()->library, &anim->getSymbol());
		dlg.ShowModal();
		Context::Instance()->stage->resetCanvas();
	}
	else if (ee::Scale9Sprite* patch9 = dynamic_cast<ee::Scale9Sprite*>(selected))
	{
		ee::Scale9Symbol& symbol = const_cast<ee::Scale9Symbol&>(patch9->getSymbol());
		libscale9::EditDialog dlg(Context::Instance()->stage, &symbol);
		dlg.ShowModal();

		std::vector<ee::Scale9Sprite*> sprites;
		Context::Instance()->stage->traverseSprites(ee::FetchAllVisitor<ee::Scale9Sprite>(sprites));
		for (size_t i = 0, n = sprites.size(); i < n; ++i)
		{
			ee::ISprite* sprite = sprites[i];
			if (&sprite->getSymbol() == &symbol)
				sprite->buildBounding();
		}

		Context::Instance()->stage->resetCanvas();
	}

	return false;
}
} // coceditor