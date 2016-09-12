#include "Image.h"
#include "RenderContext.h"
#include "StringHelper.h"
#include "Exception.h"

#include <gimg_import.h>
#include <gimg_typedef.h>

namespace gum
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
	m_filepath = filepath;

	int w, h;
	GIMG_PIXEL_FORMAT fmt;
	uint8_t* pixels = gimg_import(filepath.c_str(), &w, &h, &fmt);
	if (!pixels) {
		return false;
	}

	m_width = w;
	m_height = h;

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

	m_format = tf;
	m_id = RenderContext::Instance()->CreateTexture(pixels, w, h, tf);
	free(pixels);

	m_s2_tex->Init(w, h, m_id);
	m_s2_tex->InitOri(w, h);

	return true;
}

}