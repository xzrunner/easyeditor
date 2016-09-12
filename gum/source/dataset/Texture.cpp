#include "Texture.h"
#include "RenderContext.h"
#include "StringHelper.h"
#include "Exception.h"

#include <gimg_import.h>
#include <gimg_typedef.h>

namespace gum
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
	int w, h;
	GIMG_PIXEL_FORMAT fmt;
	uint8_t* pixels = gimg_import(filepath.c_str(), &w, &h, &fmt);
	if (!pixels) {
		return;
	}

	TEXTURE_FORMAT tf = TEXTURE_INVALID;
	switch (fmt)
	{
	case GPF_ALPHA: case GPF_LUMINANCE: case GPF_LUMINANCE_ALPHA:
		tf = TEXTURE_A8;
		break;
	case GPF_RGB:
		tf = TEXTURE_RGB;
		break;
	case GPF_RGBA:
		tf = TEXTURE_RGBA8;
		break;
	}

	m_width = w;
	m_height = h;
	m_format = tf;

	Load(pixels, m_width, m_height, m_format);
	free(pixels);
}

void Texture::Load(const uint8_t* data, int width, int height, TEXTURE_FORMAT format)
{
	m_id = RenderContext::Instance()->CreateTexture(data, width, height, format);
}

}