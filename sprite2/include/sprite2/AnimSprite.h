#ifndef _SPRITE2_ANIM_SPRITE_H_
#define _SPRITE2_ANIM_SPRITE_H_

#include "S2_Sprite.h"

namespace s2
{

class AnimSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	AnimSprite();
	AnimSprite(const AnimSprite& spr);
	
	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual AnimSprite* Clone() const;

}; // AnimSprite

}

#endif // _SPRITE2_ANIM_SPRITE_H_