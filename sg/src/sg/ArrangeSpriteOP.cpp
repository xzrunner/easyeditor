#include "ArrangeSpriteOP.h"
#include "StagePanel.h"

namespace sg
{

ArrangeSpriteOP::ArrangeSpriteOP(StagePanel* stage)
	: d2d::ArrangeSpriteFixOP(stage, stage, NULL, NULL, d2d::ArrangeSpriteConfig(false, false, false, false))
{
}

bool ArrangeSpriteOP::onMouseLeftUp(int x, int y)
{
	static_cast<StagePanel*>(m_editPanel)->GetCheckBoard().ClearRemovedCache();
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

void ArrangeSpriteOP::onDirectionKeyDown(d2d::DirectionType type)
{
	StagePanel* stage = static_cast<StagePanel*>(m_editPanel);

	std::vector<d2d::ISprite*> sprites;
	m_selection->traverse(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (int i = 0, n = sprites.size(); i < n; ++i)
	{
		d2d::ISprite* s = sprites[i];
		int row, col;
		stage->TransCoordsToGridPos(s->getPosition(), row, col);
		switch (type)
		{
		case d2d::e_up:
			++row;
			break;
		case d2d::e_down:
			--row;
			break;
		case d2d::e_left:
			--col;
			break;
		case d2d::e_right:
			++col;
			break;
		}

		d2d::Vector pos;
		stage->TransGridPosToCoords(row, col, pos);
		s->setTransform(pos, s->getAngle());
	}

	m_editPanel->Refresh();
}

}