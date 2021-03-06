#include "ImageData.h"
#include "TextureFactory.h"

#include <gimg_typedef.h>

#include <string>

namespace ee
{

ImageData::ImageData()
	: m_pixels(NULL)
	, m_width(0)
	, m_height(0)
	, m_format(0)
{
}

ImageData::ImageData(const uint8_t* pixels, int w, int h, int fmt)
	: m_pixels(pixels)
	, m_width(w)
	, m_height(h)
	, m_format(fmt)
{	
}

ImageData::~ImageData()
{
	if (m_pixels) {
		delete m_pixels;
	}
	if (!m_filepath.empty()) {
		ResourcesMgr<ImageData>* mgr = ImageDataMgr::Instance();
		mgr->RemoveItem(m_filepath);
	}
}

bool ImageData::LoadFromFile(const std::string& filepath)
{
	m_filepath = filepath;
	m_pixels = TextureFactory::Instance()->Load(filepath, m_width, m_height, m_format);
	return true;
}

void ImageData::SetContent(const uint8_t* pixels, int width, int height)
{
	if (m_pixels) {
		delete m_pixels;
	}
	m_pixels = pixels;
	m_width = width;
	m_height = height;
}

}