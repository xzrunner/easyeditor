#include "ImageSymbol.h"
#include "Bitmap.h"
#include "Image.h"
#include "ISprite.h"

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
	m_region = image->GetClippedRegion();
}

ImageSymbol::~ImageSymbol()
{
	m_bitmap->Release();
	m_image->Release();
}

ImageSymbol* ImageSymbol::Clone() const
{
	return NULL;
}

void ImageSymbol::ReloadTexture() const
{
	m_image->Reload();
}

void ImageSymbol::Draw(const Matrix& mt,
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
	m_image->Draw(mt, m_region, sprite);
}

Rect ImageSymbol::GetSize(const ISprite* sprite/* = NULL*/) const
{
	return m_region;
}

unsigned int ImageSymbol::GetTexID() const
{
	return m_image->GetTexID();
}

void ImageSymbol::LoadResources()
{
	BitmapMgr::Instance()->GetItem(m_filepath, &m_bitmap);
	ImageMgr::Instance()->GetItem(m_filepath, &m_image);

	m_region = m_image->GetClippedRegion();
}

} // d2d