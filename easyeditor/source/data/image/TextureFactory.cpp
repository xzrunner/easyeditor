#include "TextureFactory.h"
#include "TexturePacker.h"
#include "ImageData.h"
#include "Config.h"
#include "SettingData.h"

#include <gimg_import.h>
#include <gimg_typedef.h>
#include <gum/Config.h>

#include <assert.h>

namespace ee
{

TextureFactory* TextureFactory::m_instance = NULL;

TextureFactory::TextureFactory()
	: m_tp(NULL)
{
}

void TextureFactory::InitTexturePacker(const std::string& src_data_dir)
{
	m_tp = new TexturePacker(src_data_dir);
}

void TextureFactory::AddTextureFromConfig(const std::string& filepath)
{
	if (m_tp) {
		m_tp->Add(filepath);
	}
}

const uint8_t* TextureFactory::Load(const std::string& filepath, int& width, 
									int& height, int& format) const
{
	const TexturePacker::Frame* frame = NULL;
	if (!Config::Instance()->GetSettings().load_image && m_tp) {
		frame = m_tp->Query(filepath);
		if (!frame) {
			frame = m_tp->Query(ImageDataMgr::Instance()->GetDefaultSym());
		}
	}
	if (frame) {
		width = frame->src.frame.w;
		height = frame->src.frame.h;
		format = GPF_RGBA;
		return NULL;
	} else {
		uint8_t* pixels = gimg_import(filepath.c_str(), &width, &height, &format);
		if (format == GPF_RGBA && gum::Config::Instance()->GetPreMulAlpha()) {
			gimg_pre_mul_alpha(pixels, width, height);
		}
		return pixels;
	}
}

void TextureFactory::Load(const std::string& filepath, sm::i16_vec2& ori_sz, sm::i16_rect& clipped_sz) const
{
	const TexturePacker::Frame* frame = NULL;
	if (!Config::Instance()->GetSettings().load_image && m_tp) {
		frame = m_tp->Query(filepath);
	}
	if (!frame) {
		frame = m_tp->Query(ImageDataMgr::Instance()->GetDefaultSym());
	}
	if (frame) {
		const TexturePacker::FrameSrcData& src = frame->src;

		ori_sz.x = src.src_width;
		ori_sz.y = src.src_height;

		clipped_sz.xmin = src.sprite_source_size.x;
		clipped_sz.ymin = src.sprite_source_size.y;
		clipped_sz.xmax = clipped_sz.xmin + src.sprite_source_size.w;
		clipped_sz.ymax = clipped_sz.ymin + src.sprite_source_size.h;
	}
}

TextureFactory* TextureFactory::Instance()
{
	if (!m_instance) {
		m_instance = new TextureFactory;
	}
	return m_instance;
}

}