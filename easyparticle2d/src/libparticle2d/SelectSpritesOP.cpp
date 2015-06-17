#include "SelectSpritesOP.h"
#include "Sprite.h"
#include "StagePanel.h"
#include "ToolbarPanel.h"

namespace eparticle2d
{

SelectSpritesOP::SelectSpritesOP(d2d::EditPanel* editPanel, d2d::MultiSpritesImpl* spritesImpl, 
								 d2d::ViewPanelMgr* view_panel_mgr, d2d::AbstractEditCMPT* callback/* = NULL*/)
	: d2d::SelectSpritesOP(editPanel, spritesImpl, NULL, callback)
{
}

bool SelectSpritesOP::OnKeyDown(int keyCode)
{
	if (d2d::SelectSpritesOP::OnKeyDown(keyCode)) {
		return true;
	}
	OnSelected();
	return false;
}

bool SelectSpritesOP::OnMouseLeftDown(int x, int y)
{
	if (d2d::SelectSpritesOP::OnMouseLeftDown(x, y)) {
		return true;
	}
	OnSelected();
	return false;
}

bool SelectSpritesOP::OnMouseLeftUp(int x, int y)
{
	if (d2d::SelectSpritesOP::OnMouseLeftUp(x, y)) {
		return true;
	}
	OnSelected();
	return false;
}

void SelectSpritesOP::OnSelected()
{
	Symbol* symbol = NULL;
	if (m_selection->Size() == 1) 
	{
		std::vector<d2d::ISprite*> sprites;
		m_selection->Traverse(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
		Sprite* particle = dynamic_cast<Sprite*>(sprites[0]);
		if (particle) {
			symbol = const_cast<Symbol*>(&particle->getSymbol());
		}
	}

	StagePanel* stage = static_cast<StagePanel*>(m_stage);
	stage->SetSelectedSymbol(symbol);
}

}