#ifndef _SPRITE2_PARTICLE3D_SPRITE_H_
#define _SPRITE2_PARTICLE3D_SPRITE_H_

#include "S2_Sprite.h"

namespace s2
{

class Particle3dSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	Particle3dSprite();
	Particle3dSprite(Symbol* sym);

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual Particle3dSprite* Clone() const;

	VI_DUMMY_FUNC

}; // Particle3dSprite

}

#endif // _SPRITE2_PARTICLE3D_SPRITE_H_