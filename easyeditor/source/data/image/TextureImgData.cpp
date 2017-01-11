#include "TextureImgData.h"
#include "Exception.h"

#include <unirender/RenderContext.h>
#include <gimg_typedef.h>
#include <gum/RenderContext.h>

namespace ee
{

TextureImgData::TextureImgData()
	: m_width(0)
	, m_height(0)
	, m_format(0)
	, m_texid(-1)
{
}

TextureImgData::~TextureImgData()
{
	if (m_texid != -1) {
		gum::RenderContext::Instance()->GetImpl()->ReleaseTexture(m_texid);
	}
}

int TextureImgData::GetWidth() const 
{ 
	return m_width;
}

int TextureImgData::GetHeight() const 
{ 
	return m_height;
}

int TextureImgData::GetFormat() const 
{ 
	return m_format;
}

void TextureImgData::LoadFromMemory(const uint8_t* pixels, int w, int h, int fmt)
{
	ur::TEXTURE_FORMAT tf;
	switch (fmt)
	{
	case GPF_RGBA:
		tf = ur::TEXTURE_RGBA8;
		break;
	case GPF_RGB:
		tf = ur::TEXTURE_RGB;
		break;
	case GPF_LUMINANCE:
		tf = ur::TEXTURE_A8;
		break;
	default:
		throw ee::Exception("TextureImgData::Reload Unknown format %d", fmt);
	}

	m_width = w;
	m_height = h;
	m_format = fmt;

	if (pixels) {
		m_texid = gum::RenderContext::Instance()->GetImpl()->CreateTexture(pixels, w, h, tf);
	}
}

}