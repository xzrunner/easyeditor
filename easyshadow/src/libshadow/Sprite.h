#ifndef _EASYSHADOW_SPRITE_H_
#define _EASYSHADOW_SPRITE_H_

#include <ee/Sprite.h>

namespace eshadow
{

class Symbol;

class Sprite : public ee::Sprite
{
public:
	Sprite(const Sprite& spr);
	Sprite& operator = (const Sprite& spr);
	Sprite(const s2::SymPtr& sym, uint32_t id = -1);

	static ee::SprPtr Create(const std::shared_ptr<ee::Symbol>& sym);

	SPRITE_CLONE_FUNC(Sprite)

}; // Sprite

}

#endif // _EASYSHADOW_SPRITE_H_