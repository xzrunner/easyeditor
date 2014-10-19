#ifndef _LIBSKETCH_ROTATE_SPRITE_STATE_H_
#define _LIBSKETCH_ROTATE_SPRITE_STATE_H_

#include "IEditState.h"

#include <easy3d.h>

namespace libsketch
{

class Sprite;

class RotateSpriteState : public IEditState
{
public:
	RotateSpriteState(e3d::StageCanvas* canvas,
		Sprite* sprite);

	virtual void OnMousePress(const ivec2& pos);
	virtual void OnMouseRelease(const ivec2& pos);
	virtual void OnMouseMove(const ivec2& pos);	

private:
	e3d::StageCanvas* m_canvas;
	Sprite* m_sprite;

	bool m_is_open;

	Quaternion m_ori, m_pre_ori;
	ivec2 m_first_pos;

}; // RotateSpriteState

}

#endif // _LIBSKETCH_ROTATE_SPRITE_STATE_H_