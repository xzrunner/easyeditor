#ifndef _SPRITE2_ICON_SPRITE_H_
#define _SPRITE2_ICON_SPRITE_H_

#include "S2_Sprite.h"

namespace s2
{

class IconSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	IconSprite();
	IconSprite(Symbol* sym);

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual IconSprite* Clone() const;

	VI_DUMMY_FUNC

}; // IconSprite

}

#endif // _SPRITE2_ICON_SPRITE_H_