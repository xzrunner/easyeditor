#include "Symbol.h"
#include "Sprite.h"

namespace ebone
{

Symbol::Symbol()
{
}

Symbol::~Symbol()
{
}

void Symbol::Draw(const s2::RenderParams& params, const s2::Sprite* spr) const
{
}

sm::rect Symbol::GetBounding(const s2::Sprite* spr) const
{
	return sm::rect(sm::vec2(0, 0), 200, 200);
}

void Symbol::LoadResources()
{
}

}