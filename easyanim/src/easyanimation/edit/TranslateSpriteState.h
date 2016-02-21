#ifndef _EASYANIM_TRANSLATE_SPRITE_STATE_H_
#define _EASYANIM_TRANSLATE_SPRITE_STATE_H_

#include <ee/TranslateSpriteState.h>

namespace eanim
{

class TranslateSpriteState : public ee::TranslateSpriteState
{
public:
	TranslateSpriteState(ee::SpriteSelection* selection, const ee::Vector& first_pos);
	
protected:
	virtual void Translate(const ee::Vector& offset);

}; // TranslateSpriteState 

}

#endif // _EASYANIM_TRANSLATE_SPRITE_STATE_H_