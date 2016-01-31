#ifndef _SG_CHECKER_BOARD_H_
#define _SG_CHECKER_BOARD_H_

#include "config.h"



namespace sg
{

class StagePanel;

class CheckerBoard
{
public:
	CheckerBoard(StagePanel* stage);

	void AddSprite(ee::Sprite* sprite);
	void RemoveSprite(ee::Sprite* sprite);
	void Clear();

	bool IsValid(ee::Sprite* sprite) const;
	bool IsValid(const ee::Symbol& symbol, const ee::Vector& pos) const;

	void DebugDraw() const;

	void ClearRemovedCache() {
		m_mapRemoved.clear();
	}
	bool SetCachedPos(ee::Sprite* sprite) const;

	void ResetWall();

private:
	StagePanel* m_stage;

	ee::Sprite* m_grid[ROW][COL];

	std::map<ee::Sprite*, ee::Vector> m_mapSprite2Pos;

	std::map<ee::Sprite*, ee::Vector> m_mapRemoved;

}; // CheckerBoard

}

#endif // _SG_CHECKER_BOARD_H_