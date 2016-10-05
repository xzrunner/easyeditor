#ifndef _EASYSKELETON_SCALE_SPR_STATE_H_
#define _EASYSKELETON_SCALE_SPR_STATE_H_

#include <ee/ScaleSpriteState.h>

namespace eskeleton
{

class ScaleSprState : public ee::ScaleSpriteState
{
public:
	ScaleSprState(ee::Sprite* spr, const ee::SpriteCtrlNode::Node& ctrl_node);

protected:
	virtual void SetScaleTimes(const sm::vec2& st);

}; // ScaleSprState

}

#endif // _EASYSKELETON_SCALE_SPR_STATE_H_