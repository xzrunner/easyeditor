#include "SelectSpriteOP.h"
#include "StagePanel.h"
#include "Symbol.h"

namespace libsketch
{

SelectSpriteOP::SelectSpriteOP(StagePanel* stage)
	: d2d::AbstractEditOP(stage)
	, m_stage(stage)
{
	m_selection = stage->getSpriteSelection();
	m_selection->retain();
}

bool SelectSpriteOP::onMouseLeftDown(int x, int y)
{
	if (d2d::AbstractEditOP::onMouseLeftDown(x, y)) return true;

	if (m_selection->empty()) {
		SelectByPos(ivec2(x, y));
	}

	return false;
}

d2d::ISprite* SelectSpriteOP::SelectByPos(const ivec2& pos) const
{
	d2d::ISprite* selected = NULL;

	std::vector<d2d::ISprite*> sprites;
	m_stage->traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));

	for (int i = 0, n = sprites.size(); i < n; ++i)
	{
		d2d::ISprite* sprite = sprites[i];
		const Symbol& symbol = static_cast<const Symbol&>(sprite->getSymbol());
		
		const e3d::AABB& aabb = symbol.GetAABB();

//		symbol.getSize();
		m_selection->insert(sprite);
	}

	return selected;
}

}