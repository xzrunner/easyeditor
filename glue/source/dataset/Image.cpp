#include "Image.h"
#include "PngLoader.h"
#include "JpegLoader.h"
#include "RenderContext.h"
#include "StringHelper.h"
#include "Exception.h"
#include "ImageLoader.h"

namespace glue
{

Image::Image()
	: m_width(0)
	, m_height(0)
	, m_id(0)
	, m_s2_tex(new s2::Texture(0, 0, 0))
{
}

Image::~Image()
{
	RenderContext::Instance()->ReleaseTexture(m_id);
	delete m_s2_tex;
}

bool Image::LoadFromFile(const std::string& filepath)
{
	bool ret = false;

	std::string lower_path = filepath;
	StringHelper::ToLower(lower_path);
	if (lower_path.find(".png") != std::string::npos) 
	{
		TEXTURE_FORMAT tf;
		uint8_t* data = PngLoader::Read(filepath, m_width, m_height, tf);
		glue::ImageLoader::RemoveGhostPixel(data, m_width, m_height);
		glue::ImageLoader::FormatPixelsAlpha(data, m_width, m_height, 0);
		m_format = tf;
		m_id = RenderContext::Instance()->CreateTexture(data, m_width, m_height, tf);
		delete[] data;
		ret = true;
	} 
	else if (lower_path.find(".jpg") != std::string::npos) 
	{
		int c;
		const uint8_t* data = JpegLoader::Read(filepath.c_str(), m_width, m_height, c);
		switch (c)
		{
		case 3:
			m_format = TEXTURE_RGB;
			break;
		case 4:
			m_format = TEXTURE_RGBA8;
			break;
		default:
			throw Exception("Image::Load: Unknown format.");
		}

		m_id = RenderContext::Instance()->CreateTexture(data, m_width, m_height, (TEXTURE_FORMAT)m_format);
		delete[] data;
		ret = true;
	}

	if (ret) {
		m_s2_tex->Init(m_width, m_height, m_id);
		m_s2_tex->InitOri(m_width, m_height);
	}

	return ret;
}

}