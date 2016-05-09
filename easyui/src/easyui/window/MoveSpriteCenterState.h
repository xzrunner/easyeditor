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
	MoveSpriteCenterState(ecomplex::Sprite* spr, const sm::vec2& first_pos);
	virtual ~MoveSpriteCenterState();

	virtual	void OnMousePress(const sm::vec2& pos);
	virtual void OnMouseRelease(const sm::vec2& pos);
	virtual	bool OnMouseDrag(const sm::vec2& pos);

private:
	ecomplex::Sprite* m_spr;

	sm::vec2 m_first_pos, m_last_pos;
	bool m_last_pos_valid;

	bool m_dirty;

}; // MoveSpriteCenterState

}
}

#endif // _EASYUI_WINDOW_MOVE_SPRITE_CENTER_STATE_H_