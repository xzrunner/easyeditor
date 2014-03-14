#include "Symbol.h"
#include "ParticleSystem.h"

namespace eparticle3d
{

Symbol::Symbol()
	: m_ps(NULL)
{
}

Symbol::Symbol(const Symbol& s)
	: d2d::ISymbol(s)
{
	m_ps = new ParticleSystem(*s.m_ps);
}

Symbol::~Symbol()
{
	if (m_ps)
	{
		m_ps->release();
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

void Symbol::draw(const d2d::Colorf& mul, const d2d::Colorf& add,
				  const d2d::ISprite* sprite) const
{
	if (!m_ps) {
		return;
	}

	static clock_t time = 0;
	clock_t curr = clock();
	if (time != 0) {
		m_ps->update((float)(curr - time) / CLOCKS_PER_SEC);
	}
	time = curr;
}

d2d::Rect Symbol::getSize(const d2d::ISprite* sprite) const
{
	return m_region;
}

void Symbol::refresh()
{
}

void Symbol::loadResources()
{
	
}

}