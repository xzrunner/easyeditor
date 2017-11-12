#ifndef _EASYEDITOR_MOVE_SPRITE_STATE_H_
#define _EASYEDITOR_MOVE_SPRITE_STATE_H_

#include "ArrangeSpriteState.h"
#include "Sprite.h"

#include <vector>

namespace ee
{

class SpriteSelection;

class MoveSpriteState : public ArrangeSpriteState
{
public:
	MoveSpriteState(SpriteSelection* selection);

	virtual bool OnMouseMove(const sm::vec2& pos) override;

private:
	std::vector<SprPtr> m_sprs;

	sm::vec2 m_center;
	std::vector<sm::vec2> m_offset;

}; // MoveSpriteState

}

#endif // _EASYEDITOR_MOVE_SPRITE_STATE_H_