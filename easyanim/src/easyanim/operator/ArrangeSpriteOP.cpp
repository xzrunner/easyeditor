#include "ArrangeSpriteOP.h"
#include "dataset/Sprite.h"
#include "view/SpriteEditDialog.h"
#include "view/StagePanel.h"

namespace eanim
{

ArrangeSpriteOP::ArrangeSpriteOP(StagePanel* editPanel)
	: ee::ArrangeSpriteOP<ee::SelectSpritesOP>(editPanel, editPanel)
	, m_editPanel(editPanel)
{	
}

bool ArrangeSpriteOP::onMouseLeftDClick(int x, int y)
{
	if (ee::ArrangeSpriteOP<ee::SelectSpritesOP>::onMouseLeftDClick(x, y)) return true;

	ee::Vector pos = m_editPanel->transPosScreenToProject(x, y);
	ee::ISprite* selected = m_editPanel->querySpriteByPos(pos);

	Sprite* actor = static_cast<Sprite*>(selected);
	SpriteEditDialog dlg(m_editPanel, actor);
	dlg.ShowModal();
	m_editPanel->resetCanvas();

	return false;
}

} // eanim