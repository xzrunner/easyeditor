#ifndef _EASYUI_WINDOW_SPRITE_H_
#define _EASYUI_WINDOW_SPRITE_H_

#include <ee/Sprite.h>

namespace eui
{
namespace window
{

class Symbol;

class Sprite : public ee::Sprite
{
public:
	Sprite(const Sprite& spr);
	Sprite& operator = (const Sprite& spr);
	Sprite(const s2::SymPtr& sym, uint32_t id = -1);

	virtual bool Update(const s2::UpdateParams& up);

	static ee::SprPtr Create(const std::shared_ptr<ee::Symbol>& sym);

	S2_SPR_CLONE_FUNC(Sprite)

}; // Sprite

}
}

#endif // _EASYUI_WINDOW_SPRITE_H_