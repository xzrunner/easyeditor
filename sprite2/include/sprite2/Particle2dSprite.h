#ifndef _SPRITE2_PARTICLE3D_SPRITE_H_
#define _SPRITE2_PARTICLE3D_SPRITE_H_

#include "S2_Sprite.h"

namespace s2
{

class Particle2dSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	Particle2dSprite();
	Particle2dSprite(const Particle2dSprite& spr);

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual Particle2dSprite* Clone() const;

}; // Particle2dSprite

}

#endif // _SPRITE2_PARTICLE3D_SPRITE_H_