#ifndef _EASYSKELETON_ROTATE_SPR_STATE_H_
#define _EASYSKELETON_ROTATE_SPR_STATE_H_

#include <ee/RotateSpriteState.h>

namespace eskeleton
{

class RotateSprState : public ee::RotateSpriteState
{
public:
	RotateSprState(ee::SpriteSelection* selection, const sm::vec2& first_pos);

protected:
	virtual void Rotate(const sm::vec2& dst);

}; // RotateSprState

}

#endif // _EASYSKELETON_ROTATE_SPR_STATE_H_