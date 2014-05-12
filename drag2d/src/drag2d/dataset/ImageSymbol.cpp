#include "ImageSymbol.h"
#include "Bitmap.h"
#include "Image.h"
#include "ISprite.h"

#include "render/Shader.h"

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

void ImageSymbol::draw(const Colorf& mul, const Colorf& add,
					   const ISprite* sprite/* = NULL*/) const
{
	Shader* shader = Shader::Instance();
	shader->sprite();
	shader->color(mul, add);

	m_image->draw(m_region);
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
	d2d::BitmapMgr::Instance()->getItem(m_filepath, &m_bitmap);
	d2d::ImageMgr::Instance()->getItem(m_filepath, &m_image);

	m_region = m_image->getRegion();
}

} // d2d