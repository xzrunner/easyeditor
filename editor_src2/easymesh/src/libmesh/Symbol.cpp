#include "Symbol.h"

namespace emesh
{

Symbol::Symbol()
	: m_image(NULL)
{
}

Symbol::Symbol(const Symbol& s)
	: m_image(s.m_image)
	, m_mesh(s.m_mesh)
{
}

Symbol::Symbol(d2d::Image* image)
	: m_image(image)
{
}

Symbol::~Symbol()
{
}

Symbol* Symbol::clone() const 
{ 
	return new Symbol(*this);
}

void Symbol::reloadTexture() const
{
	m_image->reload();
}

void Symbol::draw(const d2d::ISprite* sprite) const
{
	m_image->draw(m_image->getRegion());
}

d2d::Rect Symbol::getSize(const d2d::ISprite* sprite) const
{
	return m_image->getRegion();
}

void Symbol::refresh()
{
}

void Symbol::loadResources()
{

}

}