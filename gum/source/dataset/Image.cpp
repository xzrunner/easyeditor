#include "Image.h"
#include "RenderContext.h"
#include "StringHelper.h"
#include "Exception.h"

#include <fault.h>
#include <gimg_import.h>
#include <gimg_typedef.h>
#include <simp/ImportStream.h>

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

void Image::LoadBin(const std::string& filepath)
{
	if (m_filepath == filepath) {
		return;
	}

	Loader loader(filepath, this);
	loader.Load();

	m_filepath = filepath;
}

// void Image::Load(const std::string& filepath, int w, int h, uint8_t* pixels)
// {
// 	m_filepath = filepath;
// 
// 	m_width = w;
// 	m_height = h;
// 
// 	m_format = TEXTURE_RGBA8;
// 	m_id = RenderContext::Instance()->CreateTexture(pixels, w, h, (TEXTURE_FORMAT)m_format);
// 
// 	m_s2_tex->Init(w, h, m_id);
// 	m_s2_tex->InitOri(w, h);
// }

/************************************************************************/
/* class Image::Loader                                                  */
/************************************************************************/

Image::Loader::Loader(const std::string& filepath, Image* img) 
	: simp::FileLoader(filepath) 
	, m_img(img)
{
}

void Image::Loader::OnLoad(simp::ImportStream& is)
{
	if (m_img->m_id != 0) {
		RenderContext::Instance()->ReleaseTexture(m_img->m_id);
	}

	uint8_t type = is.UInt8();
	switch (type)
	{
	case GIT_PNG:
		{
			int w = is.UInt16(),
				h = is.UInt16();
			const char* pixels = is.Block(w * h * 4);

		 	m_img->m_width = w;
		 	m_img->m_height = h;
		 
		 	m_img->m_format = TEXTURE_RGBA8;
		 	m_img->m_id = RenderContext::Instance()->CreateTexture((const uint8_t*)pixels, 
				w, h, (TEXTURE_FORMAT)(m_img->m_format));
		 
		 	m_img->m_s2_tex->Init(w, h, m_img->m_id);
		 	m_img->m_s2_tex->InitOri(w, h);
		}
		break;
	default:
		fault("unknown image type! \n");
	}
}

}