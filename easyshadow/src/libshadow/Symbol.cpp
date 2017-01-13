#include "Symbol.h"
#include "Shadow.h"
#include "config.h"
#include "FileIO.h"

#include <ee/Sprite.h>

#include <sprite2/RenderParams.h>
#include <gum/FilepathHelper.h>

namespace eshadow
{

Symbol::Symbol()
{
	m_shadow = new Shadow(SOFT_SHADOW_RADIUS);
}

Symbol::~Symbol()
{
	if (m_shadow) {
		m_shadow->RemoveReference();
	}
}

void Symbol::Draw(const s2::RenderParams& params, const s2::Sprite* spr) const
{
	s2::RenderParams p = params;
	if (spr) {
		p.mt = spr->GetLocalMat() * params.mt;
		p.color = spr->GetColor() * params.color;
	}
	if (m_shadow) {
		m_shadow->Draw(p.mt, p.color.mul.a);
	}
}

sm::rect Symbol::GetBounding(const s2::Sprite* spr) const
{
	if (m_shadow) {
		return m_shadow->GetRegion();
	} else {
		return sm::rect(sm::vec2(0, 0), 200, 200);
	}
}

bool Symbol::LoadResources()
{
	if (!gum::FilepathHelper::Exists(m_filepath)) {
		return false;
	}
	FileIO::LoadFromFile(m_filepath.c_str(), this);
	return true;
}

}