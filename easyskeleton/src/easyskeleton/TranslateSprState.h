#ifndef _EASYSKELETON_TRANSLATE_SPR_STATE_H_
#define _EASYSKELETON_TRANSLATE_SPR_STATE_H_

#include <ee/TranslateSpriteState.h>

namespace eskeleton
{

class TranslateSprState : public ee::TranslateSpriteState
{
public:
	TranslateSprState(ee::SpriteSelection* selection, const sm::vec2& first_pos);
	
protected:
	virtual void Translate(const sm::vec2& offset);

}; // TranslateSprState 

}

#endif // _EASYSKELETON_TRANSLATE_SPR_STATE_H_