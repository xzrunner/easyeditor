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
	: d2d::Symbol(s)
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

void Symbol::Draw(const d2d::Matrix& mt, const d2d::ColorTrans& color, 
				  const d2d::Sprite* spr, const d2d::Sprite* root) const
{
	if (m_shadow) {
		m_shadow->Draw(mt, color.multi.a);
	}
}

d2d::Rect Symbol::GetSize(const d2d::Sprite* sprite) const
{
	if (m_shadow) {
		return m_shadow->GetRegion();
	} else {
		return d2d::Rect(200, 200);
	}
}

void Symbol::LoadResources()
{
	FileIO::LoadFromFile(m_filepath.c_str(), this);
}

}