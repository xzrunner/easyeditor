#ifndef _EASYANIM_ROTATE_SPRITE_STATE_H_
#define _EASYANIM_ROTATE_SPRITE_STATE_H_

#include <ee/RotateSpriteState.h>

namespace eanim
{

class RotateSpriteState : public ee::RotateSpriteState
{
public:
	RotateSpriteState(ee::SpriteSelection* selection, const sm::vec2& first_pos);

protected:
	virtual void Rotate(const sm::vec2& dst);

}; // RotateSpriteState

}

#endif // _EASYANIM_ROTATE_SPRITE_STATE_H_