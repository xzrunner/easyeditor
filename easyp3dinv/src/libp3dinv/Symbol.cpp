#include "Symbol.h"
#include "Sprite.h"
#include "ParticleSystem.h"

#include <sprite2/RenderParams.h>

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

void Symbol::Draw(const s2::RenderParams& trans, const s2::Sprite* spr) const
{
	if (m_ps) {
		m_ps->Update(1.0f / 30);
		m_ps->Draw(trans.mt);
	}
}

void Symbol::ReloadTexture() const
{
	// fixme
}

ee::Rect Symbol::GetSize(const ee::Sprite* sprite) const
{
	return ee::Rect(200, 200);
}

void Symbol::LoadResources()
{
	if (!m_ps) {
		m_ps = new ParticleSystem;
	}
	m_ps->LoadFromFile(m_filepath);
}

}