#ifndef _SG_CHECKER_BOARD_H_
#define _SG_CHECKER_BOARD_H_

#include "config.h"

#include <drag2d.h>

namespace sg
{

class StagePanel;

class CheckerBoard
{
public:
	CheckerBoard(StagePanel* stage);

	void AddSprite(d2d::ISprite* sprite);
	void RemoveSprite(d2d::ISprite* sprite);
	void Clear();

	bool IsValid(d2d::ISprite* sprite) const;
	bool IsValid(const d2d::ISymbol& symbol, const d2d::Vector& pos) const;

	void DebugDraw() const;

	void ClearRemovedCache() {
		m_mapRemoved.clear();
	}
	bool SetCachedPos(d2d::ISprite* sprite) const;

	void ResetWall();

private:
	StagePanel* m_stage;

	d2d::ISprite* m_grid[ROW][COL];

	std::map<d2d::ISprite*, d2d::Vector> m_mapSprite2Pos;

	std::map<d2d::ISprite*, d2d::Vector> m_mapRemoved;

}; // CheckerBoard

}

#endif // _SG_CHECKER_BOARD_H_