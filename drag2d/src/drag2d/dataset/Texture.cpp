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

// Texture::Texture(const uint8_t* pixel, int width, int height)
// 	: m_texid(0)
// 	, m_channels(0)
// 	, m_width(width)
// 	, m_height(height)
// 	, m_pixels(pixel)
// {
// 	ImageLoader::loadTexture(m_texid, m_pixels, m_width, m_height, m_channels, GL_RGBA);
// }

Texture::~Texture()
{
	glDeleteTextures(1, &m_texid);

	delete m_pixels;
}

void Texture::LoadFromFile(const std::string& filepath)
{
	m_filepath = filepath;
	Reload();
}

void Texture::LoadFromMemory(const uint8_t* pixels, int w, int h, int c, int f)
{
	m_texid = 0;
	m_pixels = pixels;
	m_width = w;
	m_height = h;
	m_channels = c;

	ImageLoader::loadTexture(m_texid, m_pixels, m_width, m_height, m_channels, f);
}

void Texture::Reload()
{
	m_pixels = ImageLoader::loadTexture(m_filepath, m_width, m_height, m_texid, m_channels);
}

}