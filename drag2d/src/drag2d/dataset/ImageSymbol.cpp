#include "ImageSymbol.h"
#include "Bitmap.h"
#include "Image.h"
#include "ISprite.h"
#include "ImageData.h"

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
	InitRegion();
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
	m_image->ReloadTexture();
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
	m_image->Draw(mt, sprite);
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
	ImageData* img_data = ImageDataMgr::Instance()->GetItem(m_filepath);
	BitmapMgr::Instance()->GetItem(m_filepath, &m_bitmap);
	ImageMgr::Instance()->GetItem(m_filepath, &m_image);
	img_data->Release();

	InitRegion();
}

void ImageSymbol::InitRegion()
{
	float hw = m_image->GetClippedWidth() * 0.5f,
		hh = m_image->GetClippedHeight() * 0.5f;
	float dx = m_image->GetOffset().x,
		dy = m_image->GetOffset().y;

	m_region.xMin = -hw + dx;
	m_region.xMax =  hw + dx;
	m_region.yMin = -hh + dy;
	m_region.yMax =  hh + dy;
}

} // d2d