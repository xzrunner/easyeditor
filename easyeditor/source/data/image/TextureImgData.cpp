#include "TextureImgData.h"
#include "ImageData.h"
#include "Exception.h"

#include <unirender/RenderContext.h>
#include <gimg_typedef.h>
#include <gum/RenderContext.h>

namespace ee
{

TextureImgData::TextureImgData()
	: m_texid(-1)
	, m_img_data(NULL)
{
}

TextureImgData::~TextureImgData()
{
	if (m_texid != -1) {
		gum::RenderContext::Instance()->GetImpl()->ReleaseTexture(m_texid);
	}

	if (m_img_data) {
		ImageDataMgr::Instance()->RemoveItem(m_img_data->GetFilepath());
		m_img_data->RemoveReference();
	}
}

std::string TextureImgData::GetFilepath() const 
{
	return m_img_data->GetFilepath(); 
}

const uint8_t* TextureImgData::GetPixelData() const 
{ 
	return m_img_data->GetPixelData(); 
}

int TextureImgData::GetWidth() const 
{ 
	return m_img_data->GetWidth(); 
}

int TextureImgData::GetHeight() const 
{ 
	return m_img_data->GetHeight(); 
}

int TextureImgData::GetChannels() const 
{ 
	return m_img_data->GetChannels(); 
}

void TextureImgData::LoadFromFile(const std::string& filepath)
{
	ImageData* img_data = ImageDataMgr::Instance()->GetItem(filepath);
	if (img_data) {
		LoadFromMemory(img_data);
		img_data->RemoveReference();
	}
}

void TextureImgData::LoadFromMemory(ImageData* img_data)
{
	if (!img_data) {
		return;
	}

	cu::RefCountObjAssign(m_img_data, img_data);
	Reload();
}

void TextureImgData::Reload()
{
	if (!m_img_data->GetPixelData()) {
		return;
	}

	ur::TEXTURE_FORMAT tf;
	switch (m_img_data->GetFormat())
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
		throw ee::Exception("TextureImgData::Reload Unknown format %d", m_img_data->GetFormat());
	}

	m_texid = gum::RenderContext::Instance()->GetImpl()->CreateTexture(
		m_img_data->GetPixelData(), m_img_data->GetWidth(), m_img_data->GetHeight(), tf);
}

}