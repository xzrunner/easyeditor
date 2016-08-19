#ifndef _SPRITE2_PARTICLE2D_SPRITE_H_
#define _SPRITE2_PARTICLE2D_SPRITE_H_

#include "S2_Sprite.h"

namespace s2
{

class Particle2dSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	Particle2dSprite();
	Particle2dSprite(Symbol* sym);

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual Particle2dSprite* Clone() const;

	VI_DUMMY_FUNC

}; // Particle2dSprite

}

#endif // _SPRITE2_PARTICLE2D_SPRITE_H_