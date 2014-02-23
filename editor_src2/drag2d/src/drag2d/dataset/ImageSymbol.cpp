#include "ImageSymbol.h"
#include "Bitmap.h"
#include "Image.h"

namespace d2d
{

ImageSymbol::ImageSymbol()
	: m_image(NULL)
{
}

ImageSymbol::ImageSymbol(Image* image, const std::string& filename)
	: m_image(image)
{
	m_filepath = filename;
	m_region = image->getRegion();
}

ImageSymbol::~ImageSymbol()
{
}

ImageSymbol* ImageSymbol::clone() const
{
	return NULL;
}

void ImageSymbol::loadFromTextFile(std::ifstream& fin)
{
}

void ImageSymbol::storeToTextFile(std::ofstream& fout) const
{
}

void ImageSymbol::reloadTexture() const
{
	m_image->reload();
}

void ImageSymbol::draw(const ISprite* sprite/* = NULL*/) const
{
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