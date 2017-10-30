#ifndef _EASYEDITOR_COPY_PASTE_SPRITE_STATE_H_
#define _EASYEDITOR_COPY_PASTE_SPRITE_STATE_H_

#include "ArrangeSpriteState.h"
#include "Sprite.h"

#include <vector>

namespace ee
{

class Sprite;
class SpriteSelection;

class CopyPasteSpriteState : public ArrangeSpriteState
{
public:
	CopyPasteSpriteState(SpriteSelection* selection);

	virtual void OnMousePress(const sm::vec2& pos);
	virtual bool OnMouseDrag(const sm::vec2& pos);

private:
	sm::vec2 m_last_pos;

	std::vector<SprPtr> m_sprs;
	
}; // CopyPasteSpriteState

}

#endif // _EASYEDITOR_COPY_PASTE_SPRITE_STATE_H_