#include "SelectOP.h"
#include "StagePanel.h"
#include "Symbol.h"

namespace libsketch
{

SelectOP::SelectOP(StagePanel* stage)
	: d2d::AbstractEditOP(stage)
	, m_stage(stage)
{
}

bool SelectOP::onMouseLeftDown(int x, int y)
{
	if (d2d::AbstractEditOP::onMouseLeftDown(x, y)) return true;

	return false;
}

d2d::ISprite* SelectOP::SelectByPos(const ivec2& pos) const
{
	d2d::ISprite* selected = NULL;

	std::vector<d2d::ISprite*> sprites;
	m_stage->getSpriteSelection()->traverse(d2d::FetchAllVisitor<d2d::ISprite>(sprites));

	for (int i = 0, n = sprites.size(); i < n; ++i)
	{
		d2d::ISprite* sprite = sprites[i];
		const Symbol& symbol = static_cast<const Symbol&>(sprite->getSymbol());
//		symbol.getSize();
	}

	return selected;
}

}