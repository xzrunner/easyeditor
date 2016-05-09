#ifndef _SG_CHECKER_BOARD_H_
#define _SG_CHECKER_BOARD_H_

#include "config.h"

#include <map>

namespace ee { class Sprite; class Symbol; class Visitor; }

namespace sg
{

class StagePanel;

class CheckerBoard
{
public:
	CheckerBoard(StagePanel* stage);

	void Traverse(ee::Visitor& visitor) const;

	void AddSprite(ee::Sprite* sprite);
	void RemoveSprite(ee::Sprite* sprite);
	void Clear();

	bool IsValid(ee::Sprite* sprite) const;
	bool IsValid(const ee::Symbol& symbol, const sm::vec2& pos) const;

	void DebugDraw() const;

	void ClearRemovedCache() {
		m_map_removed.clear();
	}
	bool SetCachedPos(ee::Sprite* sprite) const;

	void ResetWall();

private:
	StagePanel* m_stage;

	ee::Sprite* m_grid[ROW][COL];

	std::map<ee::Sprite*, sm::vec2> m_map_sprite2pos;

	std::map<ee::Sprite*, sm::vec2> m_map_removed;

}; // CheckerBoard

}

#endif // _SG_CHECKER_BOARD_H_