#ifndef _EASYEDITOR_MOVE_SPRITE_STATE_H_
#define _EASYEDITOR_MOVE_SPRITE_STATE_H_

#include "ArrangeSpriteState.h"

#include <vector>

namespace ee
{

class Sprite;
class SpriteSelection;

class MoveSpriteState : public ArrangeSpriteState
{
public:
	MoveSpriteState(SpriteSelection* selection);
	virtual ~MoveSpriteState();

	virtual	bool OnMouseMove(const sm::vec2& pos);

private:
	std::vector<Sprite*> m_sprites;

	sm::vec2 m_center;
	std::vector<sm::vec2> m_offset;

}; // MoveSpriteState

}

#endif // _EASYEDITOR_MOVE_SPRITE_STATE_H_