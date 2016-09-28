#ifndef _EASYSKELETON_ROTATE_SPRITE_STATE_H_
#define _EASYSKELETON_ROTATE_SPRITE_STATE_H_

#include <ee/RotateSpriteState.h>

namespace eskeleton
{

class RotateSpriteState : public ee::RotateSpriteState
{
public:
	RotateSpriteState(ee::SpriteSelection* selection, const sm::vec2& first_pos);

protected:
	virtual void Rotate(const sm::vec2& dst);

}; // RotateSpriteState

}

#endif // _EASYSKELETON_ROTATE_SPRITE_STATE_H_