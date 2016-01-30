#include "Symbol.h"
#include "Sprite.h"
#include "ParticleSystem.h"

namespace ep3dinv
{

Symbol::Symbol()
	: m_ps(NULL)
{
}

Symbol::~Symbol()
{
	if (m_ps) {
		delete m_ps;
	}
}

void Symbol::ReloadTexture() const
{
	// fixme
}

void Symbol::Draw(const d2d::Matrix& mt, const d2d::ColorTrans& color, 
				  const d2d::Sprite* spr, const d2d::Sprite* root) const
{
	if (m_ps) {
		m_ps->Update(1.0f / 30);
		m_ps->Draw(mt);
	}
}

d2d::Rect Symbol::GetSize(const d2d::Sprite* sprite) const
{
	return d2d::Rect(200, 200);
}

void Symbol::LoadResources()
{
	if (!m_ps) {
		m_ps = new ParticleSystem;
	}
	m_ps->LoadFromFile(m_filepath);
}

}