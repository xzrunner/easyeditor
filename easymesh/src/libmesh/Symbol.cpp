#include "Symbol.h"
#include "Mesh.h"
#include "FileIO.h"

namespace emesh
{

Symbol::Symbol()
	: m_image(NULL)
	, m_mesh(NULL)
{
}

Symbol::Symbol(const Symbol& s)
	: d2d::ISymbol(s)
{
	s.m_image->retain();
	m_image = s.m_image;

	m_mesh = s.m_mesh->clone();
}

Symbol::Symbol(d2d::Image* image)
{
	image->retain();
	m_image = image;

	m_mesh = new Mesh(*image);
}

Symbol::~Symbol()
{
	if (m_image)
	{
		m_image->release();
		m_image = NULL;
	}

	if (m_mesh)
	{
		m_mesh->release();
		m_mesh = NULL;
	}
}

Symbol* Symbol::clone() const 
{ 
	return new Symbol(*this);
}

void Symbol::reloadTexture() const
{
	m_image->reload();
}

void Symbol::draw(const d2d::Colorf& mul, const d2d::Colorf& add,
				  const d2d::ISprite* sprite) const
{
	if (m_mesh)
	{
		m_mesh->drawTexture();
//		m_mesh->drawInfoUV();
	}
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
	FileIO::load(m_filepath, this);
}

}