#include "ImageSymbol.h"
#include "Bitmap.h"
#include "Image.h"
#include "ISprite.h"
#include "SpriteTools.h"

#include "render/Shader.h"
#include "render/ShaderNew.h"
#include "render/PrimitiveDraw.h"

namespace d2d
{

ImageSymbol::ImageSymbol()
	: m_image(NULL)
{
}

ImageSymbol::ImageSymbol(Image* image, const std::string& filename)
	: m_image(image)
{
	m_image->retain();

	m_filepath = filename;
	m_region = image->getRegion();
}

ImageSymbol::~ImageSymbol()
{
	m_bitmap->release();
	m_image->release();
}

ImageSymbol* ImageSymbol::clone() const
{
	return NULL;
}

void ImageSymbol::reloadTexture() const
{
	m_image->reload();
}

void ImageSymbol::draw(const Screen& scr,
					   const Matrix& mt,
					   const Colorf& mul, 
					   const Colorf& add,
					   const ISprite* sprite/* = NULL*/) const
{
// 	Shader* shader = Shader::Instance();
// 	shader->sprite();
// 	shader->color(mul, add);

	ShaderNew* shader = ShaderNew::Instance();
	shader->color(mul, add);
	m_image->draw(scr, mt, m_region);

//	SpriteTools::DrawName(sprite);
}

Rect ImageSymbol::getSize(const ISprite* sprite/* = NULL*/) const
{
	return m_region;
}

unsigned int ImageSymbol::getTextureID() const
{
	return m_image->textureID();
}

void ImageSymbol::loadResources()
{
	BitmapMgr::Instance()->getItem(m_filepath, &m_bitmap);
	ImageMgr::Instance()->getItem(m_filepath, &m_image);

	m_region = m_image->getRegion();
}

} // d2d