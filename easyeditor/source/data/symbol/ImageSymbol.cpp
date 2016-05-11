#include "ImageSymbol.h"
#include "Bitmap.h"
#include "Image.h"
#include "Sprite.h"
#include "ImageData.h"
#include "EE_RVG.h"

namespace ee
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
	if (m_bitmap) {
		m_bitmap->Release();
	}
	if (m_image) {
		m_image->Release();
	}
}

ImageSymbol* ImageSymbol::Clone() const
{
	return NULL;
}

void ImageSymbol::Draw(const s2::RenderParams& params, const Sprite* spr) const
{
	m_image->Draw(params, spr);
}

void ImageSymbol::ReloadTexture() const
{
	m_image->ReloadTexture();
}

Rect ImageSymbol::GetSize(const Sprite* sprite/* = NULL*/) const
{
	return m_region;
}

void ImageSymbol::InvalidRect(const sm::mat4& mt) const
{
	m_image->InvalidRect(mt);
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

	m_region.xmin = -hw + dx;
	m_region.xmax =  hw + dx;
	m_region.ymin = -hh + dy;
	m_region.ymax =  hh + dy;
}

}