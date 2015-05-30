#include "Symbol.h"
#include "ParticleSystem.h"
#include "FileIO.h"

namespace eparticle3d
{

Symbol::Symbol()
	: m_ps(NULL)
{
}

Symbol::Symbol(const Symbol& s)
	: d2d::ISymbol(s)
{
//	m_ps = new ParticleSystem(*s.m_ps);
}

Symbol::~Symbol()
{
	if (m_ps)
	{
		m_ps->Release();
		m_ps = NULL;
	}
}

Symbol* Symbol::clone() const
{
	return new Symbol(*this);
}

void Symbol::reloadTexture() const
{
	m_ps->reloadTexture();
}

void Symbol::draw(const d2d::Matrix& mt,
				  const d2d::Colorf& mul, 
				  const d2d::Colorf& add,
				  const d2d::Colorf& r_trans,
				  const d2d::Colorf& g_trans,
				  const d2d::Colorf& b_trans,
				  const d2d::ISprite* sprite) const
{
	if (!m_ps) {
		return;
	}

	static clock_t time = 0;
	clock_t curr = clock();
	if (time != 0) {
		if (m_ps->IsEmpty()) {
			m_ps->start();
		}
		m_ps->update((float)(curr - time) / CLOCKS_PER_SEC);
		m_ps->draw(mt);
	}
	time = curr;
}

d2d::Rect Symbol::getSize(const d2d::ISprite* sprite) const
{
//	return m_region;
	return d2d::Rect(200, 200);
}

void Symbol::loadResources()
{
	m_ps = FileIO::LoadPS(m_filepath);	
}

}