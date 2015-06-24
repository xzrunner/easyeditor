#include "Symbol.h"
#include "Sprite.h"

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

void Symbol::reloadTexture() const
{
	// fixme
}

void Symbol::draw(const d2d::Matrix& mt,
				  const d2d::Colorf& mul, 
				  const d2d::Colorf& add,
				  const d2d::Colorf& r_trans,
				  const d2d::Colorf& g_trans,
				  const d2d::Colorf& b_trans,
				  const d2d::ISprite* sprite/* = NULL*/) const
{
	if (m_ps) {
		m_ps->Draw(mt);
	}
}

d2d::Rect Symbol::getSize(const d2d::ISprite* sprite) const
{
	return d2d::Rect(200, 200);
}

void Symbol::loadResources()
{
	if (!m_ps) {
		m_ps = new ParticleSystem;
	}
	m_ps->LoadFromFile(m_filepath);
}

}