#include "Symbol.h"
#include "Shape.h"
#include "FileIO.h"
#include "Mesh.h"
#include "Strip.h"

namespace emesh
{

Symbol::Symbol()
	: m_image(NULL)
	, m_shape(NULL)
	, m_pause(false)
{
}

Symbol::Symbol(const Symbol& s)
	: d2d::ISymbol(s)
{
	s.m_image->retain();
	m_image = s.m_image;

	m_shape = s.m_shape->clone();
}

Symbol::Symbol(d2d::Image* image)
{
	image->retain();
	m_image = image;

	m_shape = new Strip(*image);
//	m_shape = new Mesh(*image);
}

Symbol::~Symbol()
{
	if (m_image)
	{
		m_image->release();
		m_image = NULL;
	}
	if (m_shape)
	{
		m_shape->release();
		m_shape = NULL;
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
	if (m_shape) {
		m_shape->DrawTexture();
		if (!m_pause) {
			m_shape->OffsetUV(0, -0.01f);			
		}
	}
}

d2d::Rect Symbol::getSize(const d2d::ISprite* sprite) const
{
	return m_image->getRegion();
}

void Symbol::refresh()
{
}

void Symbol::SetShape(Shape* shape)
{
	if (m_shape) {
		m_shape->release();
	}
	shape->retain();
	m_shape = shape;
}

const wxString& Symbol::GetImagePath() const
{
	return m_image->filepath();
}

void Symbol::LoadImage(const wxString& filepath)
{
	d2d::BitmapMgr::Instance()->getItem(filepath, &m_bitmap);
	d2d::ImageMgr::Instance()->getItem(filepath, &m_image);
}

void Symbol::loadResources()
{
	FileIO::load(m_filepath, this);
}

}