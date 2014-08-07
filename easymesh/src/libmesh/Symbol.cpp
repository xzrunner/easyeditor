#include "Symbol.h"
#include "Shape.h"
#include "FileIO.h"
#include "Sprite.h"
#include "ShapeFactory.h"

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

	m_shape = ShapeFactory::Instance()->CreateShape(*image);
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
		d2d::Shader* shader = d2d::Shader::Instance();
		shader->sprite();
		shader->color(mul, add);

		m_shape->DrawTexture();
		if (!m_pause) {
			const Sprite* mesh = static_cast<const Sprite*>(sprite);
			d2d::Vector spd = mesh->GetSpeed();
			if (spd.x != 0 && spd.y != 0) {
				m_shape->OffsetUV(spd.x, spd.y);
			}
		}
	}
}

// d2d::Rect Symbol::getSize(const d2d::ISprite* sprite) const
// {
// //	return m_image->getRegion();
// }

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

int Symbol::GetQuadSize() const 
{ 
	return m_shape->GetQuadSize(); 
}

void Symbol::loadResources()
{
	FileIO::load(m_filepath, this);
}

}