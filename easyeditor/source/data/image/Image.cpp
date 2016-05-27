#include "dev_config.h"

#include "Image.h"
#include "TextureImgData.h"
#include "Exception.h"
#include "Config.h"
#include "TextureFBO.h"
#ifdef OPEN_SCREEN_CACHE
#include "render/ScreenCache.h"
#endif // OPEN_SCREEN_CACHE
//#include "BlendShader.h"
#include "EE_DTex.h"
#include "SettingData.h"
#include "ImageData.h"
#include "TextureFactory.h"
#include "FileHelper.h"
#include "Sprite.h"
#include "Math2D.h"
#include "SpriteRenderer.h"
#include "ScreenCache.h"
#include "Camera.h"
#include "CameraMgr.h"
#include "Pseudo3DCamera.h"
#include "ImageClip.h"
#include "ImageTrim.h"

#include <shaderlab.h>
#include <sprite2/S2_Sprite.h>
#include <sprite2/Texture.h>

namespace ee
{

Image::Image()
{
	m_tex = new TextureImgData;
	m_s2_tex = new s2::Texture(0, 0, 0);
	m_xmin = m_ymin = 0;
}

Image::Image(ImageData* img_data)
{
	m_tex = new TextureImgData;
	m_tex->LoadFromMemory(img_data);

	m_ori_w = static_cast<float>(m_tex->GetWidth());
	m_ori_h = static_cast<float>(m_tex->GetHeight());
	m_offset.Set(0, 0);

	m_s2_tex = new s2::Texture(m_tex->GetWidth(), m_tex->GetHeight(), m_tex->GetTexID());
	m_xmin = m_ymin = 0;
}

Image::Image(const FBO* fbo)
{
	m_tex = new TextureFBO(fbo);
	m_s2_tex = new s2::Texture(m_tex->GetWidth(), m_tex->GetHeight(), m_tex->GetTexID());
	m_xmin = m_ymin = 0;
}

Image::~Image()
{
	ImageMgr::Instance()->RemoveItem(m_tex->GetFilepath());
	delete m_tex;
	delete m_s2_tex;
}

bool Image::LoadFromFile(const std::string& filepath)
{
	if (!Config::Instance()->GetSettings().load_image) {
		m_tex->LoadFromMemory(ImageDataMgr::Instance()->GetItem(filepath));
		int w, h;
		TextureFactory::Instance()->Load(filepath, m_ori_w, m_ori_h, w, h, m_offset);
		m_s2_tex->Init(w, h, 0);
		return true;
	}

	if (!FileHelper::IsFileExist(filepath)) {
		throw Exception("Image File: %s don't exist!", filepath.c_str());
	}

	//////////////////////////////////////////////////////////////////////////
	if (Config::Instance()->GetSettings().open_image_edge_clip) {
		LoadWithClip(filepath);
		
	} else {
		m_tex->LoadFromFile(filepath);
		m_ori_w = m_tex->GetWidth();
		m_ori_h = m_tex->GetHeight();
	}

	m_s2_tex->Init(m_tex->GetWidth(), m_tex->GetHeight(), m_tex->GetTexID());

	if (m_tex->GetWidth() == 0 || m_tex->GetHeight() == 0) {
		return true;
	}

	if (Config::Instance()->IsUseDTex() && CanUseDTex()) {
		DTex* dcb = DTex::Instance();
		dcb->LoadBegin();
		dcb->Load(this);
		dcb->LoadEnd();
	}

	return true;
}

void Image::ReloadTexture()
{
	m_tex->Reload();

	if (Config::Instance()->IsUseDTex() && CanUseDTex()) {
		DTex::Instance()->Reload(this);
	}
}

std::string Image::GetFilepath() const 
{ 
	return m_tex->GetFilepath(); 
}

unsigned int Image::GetTexID() const 
{ 
	return m_tex->GetTexID(); 
}

int Image::GetChannels() const 
{ 
	return m_tex->GetChannels(); 
}

int Image::GetOriginWidth() const 
{ 
	return m_ori_w; 
}

int Image::GetOriginHeight() const 
{ 
	return m_ori_h; 
}

int Image::GetClippedWidth() const 
{ 
	return m_tex->GetWidth(); 
}

int Image::GetClippedHeight() const 
{ 
	return m_tex->GetHeight(); 
}

const uint8_t* Image::GetPixelData() const 
{ 
	return m_tex->GetPixelData(); 
}

void Image::InvalidRect(const sm::mat4& mt) const
{
#ifdef OPEN_SCREEN_CACHE
	float hw = m_tex->GetWidth() * 0.5f,
		  hh = m_tex->GetHeight() * 0.5f;
	sm::vec2 vertices[4];
	vertices[0] = sm::vec2(-hw, -hh);
	vertices[1] = sm::vec2( hw, -hh);
	vertices[2] = sm::vec2( hw,  hh);
	vertices[3] = sm::vec2(-hw,  hh);

	float xmin = FLT_MAX, ymin = FLT_MAX, xmax = -FLT_MAX, ymax = -FLT_MAX;
	for (int i = 0; i < 4; ++i) {
		sm::vec2 pos = Math2D::TransVector(vertices[i] + m_offset, mt);
		if (pos.x < xmin) xmin = pos.x;
		if (pos.x > xmax) xmax = pos.x;
		if (pos.y < ymin) ymin = pos.y;
		if (pos.y > ymax) ymax = pos.y;
	}
	ScreenCache::Instance()->InvalidRect(xmin, ymin, xmax, ymax);
#endif // OPEN_SCREEN_CACHE
}

const ImageData* Image::GetImageData() const 
{ 
	return m_tex->GetImageData(); 
}

void Image::QueryTexcoords(float* texcoords, int* texid) const
{
	float* c2_texcoords = NULL;
	if (Config::Instance()->IsUseDTex() && CanUseDTex()) {
		c2_texcoords = DTex::Instance()->Query(this, texid);
	}
	if (c2_texcoords)
	{
		memcpy(texcoords, c2_texcoords, sizeof(float) * 8);
	}
	else
	{
		*texid = GetTexID();

		float txmin, txmax, tymin, tymax;
		txmin = tymin = 0;
		txmax = tymax = 1;

		texcoords[0] = txmin;
		texcoords[1] = tymin;
		texcoords[2] = txmax;
		texcoords[3] = tymin;
		texcoords[4] = txmax;
		texcoords[5] = tymax;
		texcoords[6] = txmin;
		texcoords[7] = tymax;
	}
}

void Image::LoadWithClip(const std::string& filepath)
{
	ImageData* img_data = ImageDataMgr::Instance()->GetItem(filepath);

	m_ori_w = img_data->GetWidth();
	m_ori_h = img_data->GetHeight();

	if (img_data->GetChannels() != 4) 
	{
		m_tex->LoadFromMemory(img_data);
	} 
	else 
	{
		ImageTrim trim(*img_data);
		sm::rect r = trim.Trim();
		sm::vec2 sz = r.Size();
		if (sz.x >= img_data->GetWidth() && sz.y >= img_data->GetHeight()) {
			m_tex->LoadFromMemory(img_data);
		} else {
			int w = img_data->GetWidth(),
				h = img_data->GetHeight();

			ImageClip clip(*img_data);
			const uint8_t* c_pixels = clip.Clip(r);

			sm::vec2 sz = r.Size();
			img_data->SetContent(c_pixels, sz.x, sz.y);
			m_tex->LoadFromMemory(img_data);

			sm::vec2 center = r.Center();
			m_offset.x = center.x - w * 0.5f;
			m_offset.y = center.y - h * 0.5f;

			m_xmin = r.xmin;
			m_ymin = r.ymin;
		}
	}

	img_data->Release();
}

bool Image::CanUseDTex() const
{
	return m_tex->GetWidth() < 512 && m_tex->GetHeight() < 512;
}

}