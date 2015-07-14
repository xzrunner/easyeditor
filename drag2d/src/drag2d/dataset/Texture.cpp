#include "Texture.h"
#include "ImageData.h"
#include "ImageLoader.h"

#include <gl/glew.h>

namespace d2d
{

Texture::Texture()
	: m_texid(0)
	, m_img_data(NULL)
{
}

Texture::~Texture()
{
	if (m_texid != 0) {
		glDeleteTextures(1, &m_texid);
	}

	if (m_img_data) {
		m_img_data->Release();
	}
}

const std::string& Texture::GetFilepath() const 
{
	return m_img_data->GetFilepath(); 
}

const uint8_t* Texture::GetPixelData() const 
{ 
	return m_img_data->GetPixelData(); 
}

int Texture::GetWidth() const 
{ 
	return m_img_data->GetWidth(); 
}

int Texture::GetHeight() const 
{ 
	return m_img_data->GetHeight(); 
}

int Texture::GetChannels() const 
{ 
	return m_img_data->GetChannels(); 
}

void Texture::LoadFromFile(const std::string& filepath)
{
	ImageData* img_data = ImageDataMgr::Instance()->GetItem(filepath);
	LoadFromMemory(img_data);
	img_data->Release();
}

void Texture::LoadFromMemory(ImageData* img_data)
{
	obj_assign((d2d::Object*&)m_img_data, img_data);
	Reload();
}

void Texture::Reload()
{
	ImageLoader::loadTexture(m_texid, m_img_data->GetPixelData(), m_img_data->GetWidth(), 
		m_img_data->GetHeight(), m_img_data->GetChannels(), m_img_data->GetFormat());
}

}