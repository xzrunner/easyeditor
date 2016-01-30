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
	: d2d::Symbol(s)
{
	s.m_image->Retain();
	m_image = s.m_image;

	m_shape = s.m_shape->Clone();
}

Symbol::Symbol(d2d::Image* image)
{
	image->Retain();
	m_image = image;

	m_shape = ShapeFactory::Instance()->CreateShape(*m_image);
}

Symbol::~Symbol()
{
	if (m_image)
	{
		m_image->Release();
		m_image = NULL;
	}
	if (m_shape)
	{
		m_shape->Release();
		m_shape = NULL;
	}
}

Symbol* Symbol::Clone() const 
{ 
	return new Symbol(*this);
}

void Symbol::ReloadTexture() const
{
	if (m_image) {
		m_image->ReloadTexture();
	}
}

void Symbol::Draw(const d2d::Matrix& mt, const d2d::ColorTrans& color, 
				  const d2d::Sprite* spr, const d2d::Sprite* root) const
{
	if (!m_shape) {
		return;
	}

	d2d::ShaderMgr* shader = d2d::ShaderMgr::Instance();
	shader->sprite();
	shader->SetSpriteColor(color);

	m_shape->DrawTexture(mt, color.multi, color.add);
	if (!m_pause && spr) 
	{
		const Sprite* s = static_cast<const Sprite*>(spr);
		d2d::Vector spd = s->GetSpeed();
		if (spd.x != 0 || spd.y != 0) {
			m_shape->OffsetUV(spd.x, spd.y);
		}
	}
}

// d2d::Rect Symbol::getSize(const d2d::Sprite* sprite) const
// {
// //	return m_image->getRegion();
// }

void Symbol::SetShape(Shape* shape)
{
	if (m_shape) {
		m_shape->Release();
	}
	shape->Retain();
	m_shape = shape;
}

std::string Symbol::GetImagePath() const
{
	return m_image->GetFilepath();
}

void Symbol::LoadImage(const std::string& filepath)
{
//	d2d::BitmapMgr::Instance()->GetItem(filepath, &m_bitmap);
	d2d::ImageMgr::Instance()->GetItem(filepath, &m_image);
}

void Symbol::CreateShape()
{
	if (m_shape) {
		m_shape->Release();
	}
	m_shape = ShapeFactory::Instance()->CreateShape(*m_image);
}

void Symbol::LoadResources()
{
	FileIO::load(m_filepath.c_str(), this);
	InitBounding();
}

void Symbol::InitBounding()
{
	m_region = m_shape->GetRegion();
}

}