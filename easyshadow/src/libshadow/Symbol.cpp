#include "Symbol.h"
#include "Shadow.h"
#include "config.h"
#include "FileIO.h"

#include <sprite2/RenderParams.h>

namespace eshadow
{

Symbol::Symbol()
{
	m_shadow = new Shadow(SOFT_SHADOW_RADIUS);
}

Symbol::Symbol(const Symbol& s)
	: ee::Symbol(s)
{
	m_shadow = new Shadow(SOFT_SHADOW_RADIUS);
}

Symbol::~Symbol()
{
	if (m_shadow) {
		m_shadow->Release();
	}
}

Symbol* Symbol::Clone() const
{
	return new Symbol(*this);
}

void Symbol::Draw(const s2::RenderParams& params, const ee::Sprite* spr) const
{
	if (m_shadow) {
		m_shadow->Draw(params.mt, params.color.mul.a);
	}
}

ee::Rect Symbol::GetSize(const ee::Sprite* sprite) const
{
	if (m_shadow) {
		return m_shadow->GetRegion();
	} else {
		return ee::Rect(200, 200);
	}
}

void Symbol::LoadResources()
{
	FileIO::LoadFromFile(m_filepath.c_str(), this);
}

}