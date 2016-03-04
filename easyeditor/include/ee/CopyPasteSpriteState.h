#ifndef _EASYEDITOR_COPY_PASTE_SPRITE_STATE_H_
#define _EASYEDITOR_COPY_PASTE_SPRITE_STATE_H_

#include "ArrangeSpriteState.h"
#include "Vector.h"

#include <vector>

namespace ee
{

class Sprite;
class SpriteSelection;

class CopyPasteSpriteState : public ArrangeSpriteState
{
public:
	CopyPasteSpriteState(SpriteSelection* selection);
	virtual ~CopyPasteSpriteState();

	virtual void OnMousePress(const Vector& pos);
	virtual bool OnMouseDrag(const Vector& pos);

private:
	Vector m_last_pos;

	std::vector<Sprite*> m_sprites;
	
}; // CopyPasteSpriteState

}

#endif // _EASYEDITOR_COPY_PASTE_SPRITE_STATE_H_