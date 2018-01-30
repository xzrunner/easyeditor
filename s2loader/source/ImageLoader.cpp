#include "s2loader/ImageLoader.h"
#include "s2loader/LoadImageTask.h"

#include <gimg_import.h>
#include <gimg_typedef.h>
#include <gimg_pvr.h>
#include <gimg_etc2.h>
#include <timp/TextureFormat.h>
#include <timp/TextureLoader.h>
#include <unirender/RenderContext.h>
#include <fs_file.h>
#include <gum/RenderContext.h>
#include <gum/Config.h>
#include <gum/ThreadPool.h>
#include <gum/Image.h>

#include <stdlib.h>

namespace s2loader
{

ImageLoader::ImageLoader(const bimp::FilePath& res_path)
	: m_res_path(res_path)
	, m_id(0)
	, m_format(0)
	, m_width(0)
	, m_height(0)
{
}

bool ImageLoader::Load()
{
	auto& filepath = m_res_path.GetFilepath();
	if (filepath.find(".ept") != CU_STR::npos ||
		filepath.find(".pkg") != CU_STR::npos) {
		return LoadBin();
	} else {
		return LoadRaw();
	}
}

bool ImageLoader::AsyncLoad(int format, int width, int height, const std::shared_ptr<gum::Image>& img)
{
	if (m_res_path.IsSingleFile() && 
		m_res_path.GetFilepath().find(".ept") == CU_STR::npos) {
		return false;
	}

	int real_fmt = format;
	if (real_fmt == timp::TEXTURE_ETC2) {
		ur::RenderContext* rc = gum::RenderContext::Instance()->GetImpl();
		if (!rc->IsSupportETC2()) {
			real_fmt = timp::TEXTURE_RGBA4;
		}
	}

	m_id = gum::RenderContext::Instance()->GetImpl()->CreateTextureID(width, height, real_fmt);
	m_format = format;
	m_width = width;
	m_height = height;

	img->LoadFromLoader(*this);

	gum::ThreadPool::Instance()->Run(LoadImageTaskMgr::Instance()->Fetch(img));

	return true;
}

bool ImageLoader::LoadRaw()
{
	int w, h, fmt;
	uint8_t* pixels = gimg_import(m_res_path.GetFilepath().c_str(), &w, &h, &fmt);
	if (!pixels) {
		return false;
	}

	if (fmt == GPF_RGBA8 && gum::Config::Instance()->GetPreMulAlpha()) {
		gimg_pre_mul_alpha(pixels, w, h);
	}

	m_width = w;
	m_height = h;

	timp::TEXTURE_FORMAT tf = timp::TEXTURE_INVALID;
	switch (fmt)
	{
	case GPF_ALPHA: case GPF_LUMINANCE: case GPF_LUMINANCE_ALPHA:
		tf = timp::TEXTURE_A8;
		break;
	case GPF_RGB:
		tf = timp::TEXTURE_RGB;
		break;
	case GPF_RGBA8:
		tf = timp::TEXTURE_RGBA8;
		break;
	}

	m_format = tf;
	m_id = gum::RenderContext::Instance()->GetImpl()->CreateTexture(pixels, w, h, tf);
	free(pixels);

	return true;
}

bool ImageLoader::LoadBin()
{
	if (m_res_path.IsSingleFile()) 
	{
		timp::TextureLoader loader(m_res_path.GetFilepath().c_str());
		loader.Load();
		return LoadBin(loader);
	} 
	else 
	{
		fs_file* file = fs_open(m_res_path.GetFilepath().c_str(), "rb");
		timp::TextureLoader loader(file, m_res_path.GetOffset());
		loader.Load();
		fs_close(file);
		return LoadBin(loader);
	}
}

bool ImageLoader::LoadBin(const timp::TextureLoader& loader)
{
	bool ret = true;

	m_format = loader.GetFormat();
	m_width  = loader.GetWidth();
	m_height = loader.GetHeight();
	switch (m_format)
	{
	case timp::TEXTURE_RGBA4: case timp::TEXTURE_RGBA8:
		m_id = gum::RenderContext::Instance()->GetImpl()->CreateTexture(static_cast<const uint8_t*>(loader.GetData()), m_width, m_height, m_format);
		break;
	case timp::TEXTURE_PVR2:
		ret = DecodePVR2(loader.GetData());
		break;
	case timp::TEXTURE_PVR4:
		ret = DecodePVR4(loader.GetData());
		break;
	case timp::TEXTURE_ETC1:
		ret = false;
		break;
	case timp::TEXTURE_ETC2:
		ret = DecodeETC2(loader.GetData());
		break;
	}

	return ret;
}

bool ImageLoader::DecodePVR2(const void* data)
{
#if defined( __APPLE__ ) && !defined(__MACOSX)
//	int internal_format = 0;
//	internal_format = GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
	m_id = gum::RenderContext::Instance()->GetImpl()->CreateTexture(data, m_width, m_height, ur::TEXTURE_PVR2);
	return true;
#else
	return false;
#endif
}

bool ImageLoader::DecodePVR4(const void* data)
{
#if defined( __APPLE__ ) && !defined(__MACOSX)
//	int internal_format = 0;
//	internal_format = GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
	m_id = gum::RenderContext::Instance()->GetImpl()->CreateTexture(data, m_width, m_height, ur::TEXTURE_PVR4);
#else
	uint8_t* rgba8 = gimg_pvr_decode_rgba8(static_cast<const uint8_t*>(data), m_width, m_height);
	uint8_t* rgba4 = gimg_rgba8_to_rgba4_dither(rgba8, m_width, m_height);
	gimg_revert_y((uint8_t*)rgba4, m_width, m_height, GPF_RGBA4);
	m_id = gum::RenderContext::Instance()->GetImpl()->CreateTexture(rgba4, m_width, m_height, timp::TEXTURE_RGBA8);
	free(rgba4);
	free(rgba8);
#endif
	return true;
}

bool ImageLoader::DecodeETC2(const void* data)
{
	ur::RenderContext* rc = gum::RenderContext::Instance()->GetImpl();
	if (rc->IsSupportETC2()) {
		m_id = rc->CreateTexture(data, m_width, m_height, timp::TEXTURE_ETC2);
	} else {
		uint8_t* rgba8 = gimg_etc2_decode_rgba8(static_cast<const uint8_t*>(data), m_width, m_height, ETC2PACKAGE_RGBA_NO_MIPMAPS);
		uint8_t* rgba4 = gimg_rgba8_to_rgba4_dither(rgba8, m_width, m_height);
		m_id = rc->CreateTexture(rgba4, m_width, m_height, timp::TEXTURE_RGBA4);
		free(rgba4);
		free(rgba8);
	}
	return true;
}

}
