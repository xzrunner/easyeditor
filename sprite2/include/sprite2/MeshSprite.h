#ifndef _SPRITE2_MESH_SPRITE_H_
#define _SPRITE2_MESH_SPRITE_H_

#include "S2_Sprite.h"

namespace s2
{

class MeshSprite : public Sprite
{
public:
	MeshSprite(void* ud);
	
	virtual bool Update(float dt) { return false; }
	
private:
	MeshSprite() {}
	const MeshSprite& operator = (const MeshSprite& spr) { return *this; }

}; // MeshSprite

}

#endif // _SPRITE2_MESH_SPRITE_H_