#ifndef _SPRITE2_COMPLEX_SPRITE_H_
#define _SPRITE2_COMPLEX_SPRITE_H_

#include "S2_Sprite.h"

namespace s2
{

class ComplexSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	ComplexSprite();
	ComplexSprite(const ComplexSprite& spr);

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual ComplexSprite* Clone() const;

	/**
	 *  @interface
	 *    Sprite
	 */
	virtual bool Update(const RenderParams& params, float dt);
	
}; // ComplexSprite

}

#endif // _SPRITE2_COMPLEX_SPRITE_H_