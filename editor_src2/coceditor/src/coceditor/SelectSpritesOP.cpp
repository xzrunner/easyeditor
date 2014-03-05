#include "SelectSpritesOP.h"
#include "StagePanel.h"
#include "LibraryPanel.h"
#include "Context.h"

#include "complex/Dialog.h"

#include <easyanim.h>
#include <easyscale9.h>

namespace coceditor
{

SelectSpritesOP::SelectSpritesOP(d2d::EditPanel* editPanel, d2d::MultiSpritesImpl* spritesImpl, 
								 d2d::PropertySettingPanel* propertyPanel, d2d::AbstractEditCMPT* callback/* = NULL*/,
								 const d2d::Colorf& color/* = d2d::Colorf(0, 0, 0)*/)
	: d2d::SelectSpritesOP(editPanel, spritesImpl, propertyPanel, callback, color)
{
}

bool SelectSpritesOP::onMouseLeftDClick(int x, int y)
{
	if (d2d::SelectSpritesOP::onMouseLeftDClick(x, y)) return true;

	d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
	d2d::ISprite* selected = m_spritesImpl->querySpriteByPos(pos);
	if (complex::Sprite* complex = dynamic_cast<complex::Sprite*>(selected))
	{
		complex::Dialog dlg(Context::Instance()->stage, 
			const_cast<complex::Symbol*>(&complex->getSymbol()), Context::Instance()->property);
		dlg.ShowModal();
		Context::Instance()->stage->resetCanvas();
	}
	else if (d2d::AnimSprite* anim = dynamic_cast<d2d::AnimSprite*>(selected))
	{
		libanim::PreviewDialog dlg(Context::Instance()->stage, Context::Instance()->library, &anim->getSymbol());
		dlg.ShowModal();
		Context::Instance()->stage->resetCanvas();
	}
	else if (d2d::Scale9Sprite* patch9 = dynamic_cast<d2d::Scale9Sprite*>(selected))
	{
		d2d::Scale9Symbol& symbol = const_cast<d2d::Scale9Symbol&>(patch9->getSymbol());
		libscale9::EditDialog dlg(Context::Instance()->stage, &symbol);
		dlg.ShowModal();

		std::vector<d2d::Scale9Sprite*> sprites;
		Context::Instance()->stage->traverseSprites(d2d::FetchAllVisitor<d2d::Scale9Sprite>(sprites));
		for (size_t i = 0, n = sprites.size(); i < n; ++i)
		{
			d2d::ISprite* sprite = sprites[i];
			if (&sprite->getSymbol() == &symbol)
				sprite->buildBounding();
		}

		Context::Instance()->stage->resetCanvas();
	}

	return false;
}
} // coceditor