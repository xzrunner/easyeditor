#include "SelectSpritesOP.h"
#include "Sprite.h"
#include "StagePanel.h"
#include "ToolbarPanel.h"

namespace eparticle2d
{

SelectSpritesOP::SelectSpritesOP(d2d::EditPanel* editPanel, d2d::MultiSpritesImpl* spritesImpl, 
								 d2d::PropertySettingPanel* propertyPanel, d2d::AbstractEditCMPT* callback/* = NULL*/)
	: d2d::SelectSpritesOP(editPanel, spritesImpl, propertyPanel, callback)
{
}

bool SelectSpritesOP::onKeyDown(int keyCode)
{
	if (d2d::SelectSpritesOP::onKeyDown(keyCode)) {
		return true;
	}
	OnSelected();
	return false;
}

bool SelectSpritesOP::onMouseLeftDown(int x, int y)
{
	if (d2d::SelectSpritesOP::onMouseLeftDown(x, y)) {
		return true;
	}
	OnSelected();
	return false;
}

bool SelectSpritesOP::onMouseLeftUp(int x, int y)
{
	if (d2d::SelectSpritesOP::onMouseLeftUp(x, y)) {
		return true;
	}
	OnSelected();
	return false;
}

void SelectSpritesOP::OnSelected()
{
	Symbol* symbol = NULL;
	if (m_selection->size() == 1) 
	{
		std::vector<d2d::ISprite*> sprites;
		m_selection->traverse(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
		Sprite* particle = dynamic_cast<Sprite*>(sprites[0]);
		if (particle) {
			symbol = const_cast<Symbol*>(&particle->getSymbol());
		}
	}

	StagePanel* stage = static_cast<StagePanel*>(m_editPanel);
	stage->SetSelectedSymbol(symbol);
}

}