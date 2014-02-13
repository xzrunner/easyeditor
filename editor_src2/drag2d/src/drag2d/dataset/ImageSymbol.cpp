#include "ImageSymbol.h"
#include "Bitmap.h"

namespace d2d
{

ImageSymbol::ImageSymbol()
	: m_image(NULL)
{
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
	m_image->draw();
}

Rect ImageSymbol::getSize(const ISprite* sprite/* = NULL*/) const
{
	return m_image->getRegion();
}

unsigned int ImageSymbol::getTextureID() const
{
	return m_image->textureID();
}

void ImageSymbol::loadResources()
{
	const Bitmap* init = BitmapMgr::Instance()->getItem(m_filepath);
	if (m_bitmap != init) delete m_bitmap;
	m_bitmap = const_cast<Bitmap*>(init);

	m_image = ImageMgr::Instance()->getItem(m_filepath);
}

} // d2d