#ifndef _EASYUI_WINDOW_MOVE_SPRITE_CENTER_STATE_H_
#define _EASYUI_WINDOW_MOVE_SPRITE_CENTER_STATE_H_

#include <ee/ArrangeSpriteState.h>
#include <ee/Sprite.h>

namespace ecomplex { class Sprite; }

namespace eui
{
namespace window
{

class MoveSpriteCenterState : public ee::ArrangeSpriteState
{
public:
	MoveSpriteCenterState(ecomplex::Sprite* spr, const ee::Vector& first_pos);
	virtual ~MoveSpriteCenterState();

	virtual	void OnMousePress(const ee::Vector& pos);
	virtual void OnMouseRelease(const ee::Vector& pos);
	virtual	bool OnMouseDrag(const ee::Vector& pos);

private:
	ecomplex::Sprite* m_spr;

	ee::Vector m_first_pos, m_last_pos;

	bool m_dirty;

}; // MoveSpriteCenterState

}
}

#endif // _EASYUI_WINDOW_MOVE_SPRITE_CENTER_STATE_H_