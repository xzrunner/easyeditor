#ifndef _EASYTRAIL_SPRITE_H_
#define _EASYTRAIL_SPRITE_H_

#include <ee/Sprite.h>

#include <sprite2/TrailSprite.h>

namespace etrail
{

class Symbol;

class Sprite : public s2::TrailSprite, public ee::Sprite
{
public:
	Sprite();
	Sprite(const Sprite& spr);
	Sprite& operator = (const Sprite& spr);
	Sprite(Symbol* sym);

	static ee::Sprite* Create(ee::Symbol* sym);

}; // Sprite

}

#endif // _EASYTRAIL_SPRITE_H_