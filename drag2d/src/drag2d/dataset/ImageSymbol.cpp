#include "ImageSymbol.h"
#include "Bitmap.h"
#include "Image.h"
#include "ISprite.h"
#include "SpriteTools.h"

#include "render/Shader.h"
#include "render/ShaderMgr.h"
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
	m_image->Retain();

	m_filepath = filename;
	m_region = image->getRegion();
}

ImageSymbol::~ImageSymbol()
{
	m_bitmap->Release();
	m_image->Release();
}

ImageSymbol* ImageSymbol::clone() const
{
	return NULL;
}

void ImageSymbol::reloadTexture() const
{
	m_image->reload();
}

void ImageSymbol::draw(const Matrix& mt,
					   const Colorf& mul, 
					   const Colorf& add,
					   const Colorf& r_trans,
					   const Colorf& g_trans,
					   const Colorf& b_trans,
					   const ISprite* sprite/* = NULL*/) const
{
	ShaderMgr* shader = ShaderMgr::Instance();
	shader->SetSpriteColor(mul, add);
	shader->SetSpriteColorTrans(r_trans, g_trans, b_trans);
	if (sprite) {
		shader->SetBlendMode(sprite->GetBlendMode());
	}
	
	m_image->draw(mt, m_region, sprite);
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