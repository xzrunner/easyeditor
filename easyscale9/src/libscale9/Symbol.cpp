#include "Symbol.h"
#include "Sprite.h"
#include "config.h"

#include <ee/StringHelper.h>

#include <sprite2/RenderParams.h>

namespace escale9
{

Symbol::Symbol()
{
	static int id = 0;
	m_name = FILE_TAG + ee::StringHelper::ToString(id++);
}

void Symbol::Draw(const s2::RenderParams& params, const s2::Sprite* spr) const
{
	s2::RenderParams p = params;
	if (spr) {
		p.mt = dynamic_cast<const ee::Sprite*>(spr)->GetTransMatrix() * params.mt;
		p.color = spr->Color() * params.color;
	}
	m_data.Draw(p, spr);
}

sm::rect Symbol::GetBounding(const s2::Sprite* sprite/* = NULL*/) const
{
	float w, h;
	if (spr) {
		const Sprite* spr = dynamic_cast<const Sprite*>(spr);
		spr->GetSize(w, h);
	} else {
		m_data.GetSize(w, h);
	}
	return sm::rect(sm::vec2(0, 0), w, h);
}

void Symbol::ReloadTexture() const
{
	m_data.ReloadTexture();
}

void Symbol::ResizeScale9(float width, float height)
{
	m_data.Resize(width, height);
}

void Symbol::ComposeFromSprites(ee::Sprite* sprs[3][3], float width, float height)
{
	m_data.Compose(sprs, width, height);
}

void Symbol::LoadResources()
{
	m_data.LoadFromFile(m_filepath);
}

}