#ifndef _EASYANIM_TRANSLATE_SPRITE_STATE_H_
#define _EASYANIM_TRANSLATE_SPRITE_STATE_H_

#include <drag2d.h>

namespace eanim
{

class TranslateSpriteState : public d2d::TranslateSpriteState
{
public:
	TranslateSpriteState(d2d::SpriteSelection* selection, const d2d::Vector& first_pos);
	
protected:
	virtual void Translate(const d2d::Vector& offset);

}; // TranslateSpriteState 

}

#endif // _EASYANIM_TRANSLATE_SPRITE_STATE_H_