#ifndef _SG_CHECKER_BOARD_H_
#define _SG_CHECKER_BOARD_H_

#include "config.h"

#include <ee/Visitor.h>

#include <SM_Vector.h>

#include <map>

namespace ee { class Sprite; class Symbol; }

namespace sg
{

class StagePanel;

class CheckerBoard
{
public:
	CheckerBoard(StagePanel* stage);

	void Traverse(ee::RefVisitor<ee::Sprite>& visitor) const;

	void AddSprite(const ee::SprPtr& spr);
	void RemoveSprite(const ee::SprPtr& spr);
	void Clear();

	bool IsValid(const ee::SprPtr& spr) const;
	bool IsValid(const ee::Symbol& sym, const sm::vec2& pos) const;

	void DebugDraw() const;

	void ClearRemovedCache() {
		m_map_removed.clear();
	}
	bool SetCachedPos(const ee::SprPtr& spr) const;

	void ResetWall();

private:
	StagePanel* m_stage;

	ee::SprPtr m_grid[ROW][COL];

	std::map<ee::SprPtr, sm::vec2> m_map_sprite2pos;

	std::map<ee::SprPtr, sm::vec2> m_map_removed;

}; // CheckerBoard

}

#endif // _SG_CHECKER_BOARD_H_