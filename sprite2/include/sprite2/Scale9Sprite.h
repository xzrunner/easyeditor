#ifndef _SPRITE2_SCALE9_SPRITE_H_
#define _SPRITE2_IMAGE_SPRITE_H_

#include "S2_Sprite.h"

namespace s2
{

class RenderParams;

class Scale9Sprite : public Sprite
{
public:
	Scale9Sprite(void* ud);
	Scale9Sprite(const Scale9Sprite& spr, void* ud);

	virtual bool Update(float dt) { return false; }
	
	void Draw(const RenderParams& params) const;

private:
	Scale9Sprite() {}
	const Scale9Sprite& operator = (const Scale9Sprite& spr) { return *this; }

private:
	float m_width, m_height;

	// 0 1 2
	// 3 4 5
	// 6 7 9
	Sprite* m_grids[9];

}; // Scale9Sprite

}

#endif // _SPRITE2_SCALE9_SPRITE_H_