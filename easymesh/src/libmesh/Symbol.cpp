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
		m_image->Reload();
	}
}

void Symbol::Draw(const d2d::Matrix& mt,
				  const d2d::Colorf& mul, 
				  const d2d::Colorf& add,
				  const d2d::Colorf& r_trans,
				  const d2d::Colorf& g_trans,
				  const d2d::Colorf& b_trans,
				  const d2d::ISprite* sprite) const
{
 	if (m_shape) {
 		d2d::ShaderMgr* shader = d2d::ShaderMgr::Instance();
 		shader->sprite();
		shader->SetSpriteColor(mul, add);
		shader->SetSpriteColorTrans(r_trans, g_trans, b_trans);

 		m_shape->DrawTexture(mt, mul, add);
  		if (!m_pause && sprite) 
		{
			const Sprite* s = static_cast<const Sprite*>(sprite);
  			d2d::Vector spd = s->GetSpeed();
  			if (spd.x != 0 || spd.y != 0) {
  				m_shape->OffsetUV(spd.x, spd.y);
  			}
  		}
 	}
}

// d2d::Rect Symbol::getSize(const d2d::ISprite* sprite) const
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

const std::string& Symbol::GetImagePath() const
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