#include "ArrangeSpriteOP.h"
#include "dataset/Sprite.h"
#include "view/SpriteEditDialog.h"
#include "view/StagePanel.h"

namespace eanim
{

ArrangeSpriteOP::ArrangeSpriteOP(StagePanel* editPanel)
	: d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>(editPanel, editPanel)
	, m_editPanel(editPanel)
{	
}

bool ArrangeSpriteOP::onMouseLeftDClick(int x, int y)
{
	if (d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>::onMouseLeftDClick(x, y)) return true;

	d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
	d2d::ISprite* selected = m_editPanel->querySpriteByPos(pos);

	Sprite* actor = static_cast<Sprite*>(selected);
	SpriteEditDialog dlg(m_editPanel, actor);
	dlg.ShowModal();
	m_editPanel->resetCanvas();

	return false;
}

} // eanim