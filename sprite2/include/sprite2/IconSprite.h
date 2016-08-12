#ifndef _SPRITE2_ICON_SPRITE_H_
#define _SPRITE2_ICON_SPRITE_H_

#include "S2_Sprite.h"

namespace s2
{

class IconSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	IconSprite();
	IconSprite(const IconSprite& spr);

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual IconSprite* Clone() const;

}; // IconSprite

}

#endif // _SPRITE2_ICON_SPRITE_H_