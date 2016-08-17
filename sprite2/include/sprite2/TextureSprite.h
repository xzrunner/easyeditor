#ifndef _SPRITE2_TEXTURE_SPRITE_H_
#define _SPRITE2_TEXTURE_SPRITE_H_

#include "S2_Sprite.h"

namespace s2
{

class TextureSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	TextureSprite() {}
	TextureSprite(Symbol* sym) : Sprite(sym) {}

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual TextureSprite* Clone() const;

}; // TextureSprite

}

#endif // _SPRITE2_TEXTURE_SPRITE_H_