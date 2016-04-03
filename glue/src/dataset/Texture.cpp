#include "Texture.h"
#include "PngLoader.h"
#include "JpegLoader.h"

#include "render/RenderContext.h"
#include "utility/StringHelper.h"
#include "utility/Exception.h"

namespace glue
{

Texture::Texture()
	: m_width(0)
	, m_height(0)
	, m_id(0)
{
}

Texture::~Texture()
{
	RenderContext::Instance()->ReleaseTexture(m_id);
}

void Texture::Load(const std::string& filepath)
{
	std::string lower_path = filepath;
	StringHelper::ToLower(lower_path);
	if (lower_path.find(".png") != std::string::npos) 
	{
		const uint8_t* data = PngLoader::Read(filepath, m_width, m_height, m_format);
		Load(data, m_width, m_height, m_format);
		delete[] data;
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
			throw Exception("Texture::Load: Unknown format.");
		}

		Load(data, m_width, m_height, m_format);
		delete[] data;
	}
}

void Texture::Load(const uint8_t* data, int width, int height, TEXTURE_FORMAT format)
{
	m_id = RenderContext::Instance()->CreateTexture(data, width, height, format);
}

}