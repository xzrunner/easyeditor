#ifndef _EASYP3DINV_SPRITE_H_
#define _EASYP3DINV_SPRITE_H_

#include <ee/Sprite.h>

namespace ep3dinv
{

class Symbol;

class Sprite : public ee::Sprite
{
public:
	Sprite(const Sprite& spr);
	Sprite& operator = (const Sprite& spr);
	Sprite(const s2::SymPtr& sym, uint32_t id = -1);
	virtual ~Sprite();

	static ee::SprPtr Create(const std::shared_ptr<ee::Symbol>& sym);

	S2_SPR_CLONE_FUNC(Sprite)

}; // Sprite

}

#endif // _EASYP3DINV_SPRITE_H_