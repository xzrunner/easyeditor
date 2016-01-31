#include "Symbol.h"
#include "Shadow.h"
#include "config.h"
#include "FileIO.h"

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

void Symbol::ReloadTexture() const
{
}

void Symbol::Draw(const ee::Matrix& mt, const ee::ColorTrans& color, 
				  const ee::Sprite* spr, const ee::Sprite* root) const
{
	if (m_shadow) {
		m_shadow->Draw(mt, color.multi.a);
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