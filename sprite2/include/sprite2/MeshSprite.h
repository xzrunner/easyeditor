#ifndef _SPRITE2_MESH_SPRITE_H_
#define _SPRITE2_MESH_SPRITE_H_

#include "S2_Sprite.h"

namespace s2
{

class MeshSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	MeshSprite();
	MeshSprite(const MeshSprite& spr);

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual MeshSprite* Clone() const;

}; // MeshSprite

}

#endif // _SPRITE2_MESH_SPRITE_H_