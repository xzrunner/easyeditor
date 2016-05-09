#include "TextureFactory.h"
#include "TexturePacker.h"
#include "ImageLoader.h"
#include "Config.h"
#include "SettingData.h"

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
									int& height, int& channels, int& format) const
{
	const TexturePacker::Frame* frame = NULL;
	if (!Config::Instance()->GetSettings().load_image && m_tp) {
		frame = m_tp->Query(filepath);
	}
	if (frame) {
		width = frame->src.frame.w;
		height = frame->src.frame.h;
		channels = 4;
		format = 6408;
		return NULL;
	} else {
		return ImageLoader::FileToPixels(filepath, width, height, channels, format);
	}
}

void TextureFactory::Load(const std::string& filepath, float& ori_w, 
						  float& ori_h, sm::vec2& offset) const
{
	const TexturePacker::Frame* frame = NULL;
	if (!Config::Instance()->GetSettings().load_image && m_tp) {
		frame = m_tp->Query(filepath);
	}
	if (frame) {
		const TexturePacker::FrameSrcData& src = frame->src;

		ori_w = static_cast<float>(src.src_width);
		ori_h = static_cast<float>(src.src_height);

		offset.x = src.sprite_source_size.x + src.sprite_source_size.w * 0.5f - ori_w * 0.5f;
		offset.y = -(src.sprite_source_size.y + src.sprite_source_size.h * 0.5f - ori_h * 0.5f);
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