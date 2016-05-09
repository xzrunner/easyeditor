#ifndef _EASYANIM_TRANSLATE_SPRITE_STATE_H_
#define _EASYANIM_TRANSLATE_SPRITE_STATE_H_

#include <ee/TranslateSpriteState.h>

namespace eanim
{

class TranslateSpriteState : public ee::TranslateSpriteState
{
public:
	TranslateSpriteState(ee::SpriteSelection* selection, const sm::vec2& first_pos);
	
protected:
	virtual void Translate(const sm::vec2& offset);

}; // TranslateSpriteState 

}

#endif // _EASYANIM_TRANSLATE_SPRITE_STATE_H_