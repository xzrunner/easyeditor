#ifndef _SPRITE2_ANIM_SPRITE_H_
#define _SPRITE2_ANIM_SPRITE_H_

#include "S2_Sprite.h"

namespace s2
{

class AnimSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	AnimSprite();
	AnimSprite(Symbol* sym);

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual AnimSprite* Clone() const;

	VI_DUMMY_FUNC

}; // AnimSprite

}

#endif // _SPRITE2_ANIM_SPRITE_H_