#include "Scale9Symbol.h"
#include "RenderParams.h"
#include "S2_Sprite.h"
#include "Scale9Sprite.h"

#include <assert.h>

namespace s2
{

Scale9Symbol::Scale9Symbol()
{
}

void Scale9Symbol::Draw(const RenderParams& params, const Sprite* spr) const
{
	if (spr) {
		RenderParams p = params;
		p.mt = spr->GetTransMatrix() * params.mt;
		p.color = spr->Color() * params.color;
		VI_DOWNCASTING<const Scale9Sprite*>(spr)->GetScale9().Draw(p);
	} else {
		m_s9.Draw(params);
	}
}

sm::rect Scale9Symbol::GetBounding(const Sprite* spr) const
{
	sm::vec2 sz;
	if (spr) {
		sz = VI_DOWNCASTING<const Scale9Sprite*>(spr)->GetScale9().GetSize();
	} else {
		sz = m_s9.GetSize();
	}
	return sm::rect(sz.x, sz.y);
}

void Scale9Symbol::Resize(float width, float height)
{
	m_s9.SetSize(width, height);
}

}