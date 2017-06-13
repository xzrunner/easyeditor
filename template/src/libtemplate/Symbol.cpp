#include "Symbol.h"
#include "Sprite.h"

#include <gum/FilepathHelper.h>

namespace etemplate
{

Symbol::Symbol()
{
}

Symbol::~Symbol()
{
}

s2::RenderReturn Symbol::Draw(const s2::RenderParams& params, const s2::Sprite* spr) const
{
	return s2::RENDER_NO_DATA;
}

sm::rect Symbol::GetBoundingImpl(const s2::Sprite* spr, const s2::Actor* actor, bool cache) const
{
	return sm::rect(sm::vec2(0, 0), 200, 200);
}

bool Symbol::LoadResources()
{
	if (!gum::FilepathHelper::Exists(m_filepath)) {
		return false;
	}

	return true;
}

}