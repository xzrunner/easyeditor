#include "ArrangeSpriteOP.h"
#include "StagePanel.h"

namespace sg
{

ArrangeSpriteOP::ArrangeSpriteOP(StagePanel* stage)
	: d2d::ArrangeSpriteFixOP(stage, stage, NULL, NULL, false, false)
{
}

bool ArrangeSpriteOP::onMouseLeftUp(int x, int y)
{
	static_cast<StagePanel*>(m_editPanel)->getCheckBoard().ClearRemovedCache();
	return d2d::ArrangeSpriteFixOP::onMouseLeftUp(x, y);
}

bool ArrangeSpriteOP::onMouseLeftDClick(int x, int y)
{
	if (d2d::ArrangeSpriteFixOP::onMouseLeftDClick(x, y)) {
		return true;
	}

	StagePanel* stage = static_cast<StagePanel*>(m_editPanel);

	d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
	d2d::ISprite* selected = stage->querySpriteByPos(pos);
	if (!selected) {
		return false;
	}

	std::vector<d2d::ISprite*> sprites;
	stage->traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (int i = 0, n = sprites.size(); i < n; ++i)
	{
		d2d::ISprite* s = sprites[i];
		if (&s->getSymbol() == &selected->getSymbol()) {
			stage->getSpriteSelection()->insert(s);
		}
	}

	return false;
}

}