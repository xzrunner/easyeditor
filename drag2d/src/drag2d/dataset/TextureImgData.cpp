#include "TextureImgData.h"
#include "ImageData.h"
#include "ImageLoader.h"

#include <gl/glew.h>

namespace d2d
{

TextureImgData::TextureImgData()
	: m_texid(0)
	, m_img_data(NULL)
{
}

TextureImgData::~TextureImgData()
{
	if (m_texid != 0) {
		glDeleteTextures(1, &m_texid);
	}

	if (m_img_data) {
		m_img_data->Release();
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
		img_data->Release();
	}
}

void TextureImgData::LoadFromMemory(ImageData* img_data)
{
	if (!img_data) {
		return;
	}

	obj_assign((d2d::Object*&)m_img_data, img_data);
	Reload();
}

void TextureImgData::Reload()
{
	ImageLoader::loadTexture(m_texid, m_img_data->GetPixelData(), m_img_data->GetWidth(), 
		m_img_data->GetHeight(), m_img_data->GetChannels(), m_img_data->GetFormat());
}

}