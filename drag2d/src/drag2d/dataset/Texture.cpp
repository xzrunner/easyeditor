#include "Texture.h"

#include "ImageLoader.h"

#include <gl/glew.h>

namespace d2d
{

Texture::Texture()
	: m_texid(0)
	, m_channels(0)
	, m_width(0)
	, m_height(0)
	, m_pixels(NULL)
{
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_texid);

	delete m_pixels;
}

void Texture::LoadFromFile(const std::string& filepath)
{
	m_filepath = filepath;
	m_pixels = ImageLoader::loadTexture(m_filepath, m_width, m_height, m_texid, m_channels);
	m_format = GL_RGBA;
}

void Texture::LoadFromMemory(const uint8_t* pixels, int w, int h, int c, int f)
{
	m_texid = 0;
	m_pixels = pixels;
	m_width = w;
	m_height = h;
	m_channels = c;
	m_format = f;

//	ImageLoader::loadTexture(m_texid, m_pixels, m_width, m_height, m_channels, m_format);
	Reload();
}

void Texture::Reload()
{
	ImageLoader::loadTexture(m_texid, m_pixels, m_width, m_height, m_channels, m_format);
}

}