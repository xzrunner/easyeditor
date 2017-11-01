#ifndef _LIBANIS_SPRITE_H_
#define _LIBANIS_SPRITE_H_

#include <ee/Sprite.h>

namespace libanis
{

class Symbol;

class Sprite : public ee::Sprite
{
public:
	Sprite(const Sprite& spr);
	Sprite& operator = (const Sprite& spr);
	Sprite(const s2::SymPtr& sym, uint32_t id = -1);

	static ee::SprPtr Create(const std::shared_ptr<ee::Symbol>& sym);

private:
	int m_idx;

	S2_SPR_CLONE_FUNC(Sprite)

}; // Sprite

}

#endif // _LIBANIS_SPRITE_H_