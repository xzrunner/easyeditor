#include "Symbol.h"
#include "Sprite.h"

namespace etemplate
{

Symbol::Symbol()
{
}

Symbol::~Symbol()
{
}

void Symbol::Draw(const s2::RenderParams& params, const ee::Sprite* spr) const
{
}

ee::Rect Symbol::GetSize(const ee::Sprite* sprite) const
{
	return ee::Rect(200, 200);
}

void Symbol::LoadResources()
{
}

}