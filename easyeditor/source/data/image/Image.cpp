#include "dev_config.h"

#include "Image.h"
#include "TextureImgData.h"
#include "Exception.h"
#include "Config.h"
#include "TextureRT.h"
//#include "BlendShader.h"
#include "SettingData.h"
#include "TextureFactory.h"
#include "FileHelper.h"
#include "Sprite.h"
#include "Math2D.h"
#include "SpriteRenderer.h"
#include "ImageData.h"

#include <gimg_typedef.h>
#include <pimg/Condense.h>
#include <sprite2/S2_Sprite.h>
#include <sprite2/S2_Texture.h>
#include <gum/GUM_DTex.h>

namespace ee
{

Image::Image()
{
	m_tex = new TextureImgData;
	m_s2_tex = new s2::Texture(0, 0, 0);
}

Image::Image(const uint8_t* pixels, int w, int h, int fmt)
{
	m_tex = new TextureImgData;
	m_tex->LoadFromMemory(pixels, w, h, fmt);

	m_ori_sz.x = static_cast<float>(m_tex->GetWidth());
	m_ori_sz.y = static_cast<float>(m_tex->GetHeight());
	m_clipped_region.xmin = m_clipped_region.ymin = 0;
	m_clipped_region.xmax = m_ori_sz.x;
	m_clipped_region.ymax = m_ori_sz.y;

	m_s2_tex = new s2::Texture(m_tex->GetWidth(), m_tex->GetHeight(), m_tex->GetTexID());
}

Image::Image(const s2::RenderTarget* rt)
{
	m_tex = new TextureRT(rt);
	m_s2_tex = new s2::Texture(m_tex->GetWidth(), m_tex->GetHeight(), m_tex->GetTexID());
}

Image::~Image()
{
	if (!m_filepath.empty()) {
		ImageMgr::Instance()->RemoveItem(m_filepath);
	}
	delete m_tex;
	m_s2_tex->RemoveReference();
}

bool Image::LoadFromFile(const std::string& filepath)
{
	m_filepath = filepath;

	if (!Config::Instance()->GetSettings().load_image) 
	{
		ImageData* img_data = ImageDataMgr::Instance()->GetItem(filepath);
		m_tex->LoadFromMemory(img_data->GetPixelData(), img_data->GetWidth(), img_data->GetHeight(), img_data->GetFormat());
		img_data->RemoveReference();

		TextureFactory::Instance()->Load(filepath, m_ori_sz, m_clipped_region);
		m_s2_tex->Init(m_clipped_region.Width(), m_clipped_region.Height(), 0);
		m_s2_tex->InitOri(m_ori_sz.x, m_ori_sz.y);

		return true;
	}

	if (!FileHelper::IsFileExist(filepath)) {
		throw Exception("Image File: %s don't exist!", filepath.c_str());
	}

	//////////////////////////////////////////////////////////////////////////
	if (Config::Instance()->GetSettings().open_image_edge_clip) {
		LoadWithClip(filepath);
	} else {
		ImageData* img_data = ImageDataMgr::Instance()->GetItem(filepath);
		m_tex->LoadFromMemory(img_data->GetPixelData(), img_data->GetWidth(), img_data->GetHeight(), img_data->GetFormat());
		img_data->RemoveReference();
		m_ori_sz.x = m_tex->GetWidth();
		m_ori_sz.y = m_tex->GetHeight();
		m_clipped_region.xmin = m_clipped_region.ymin = 0;
		m_clipped_region.xmax = m_ori_sz.x;
		m_clipped_region.ymax = m_ori_sz.y;
	}

	m_s2_tex->Init(m_tex->GetWidth(), m_tex->GetHeight(), m_tex->GetTexID());
	m_s2_tex->InitOri(m_ori_sz.x, m_ori_sz.y);

	if (m_tex->GetWidth() == 0 || m_tex->GetHeight() == 0) {
		return true;
	}

	if (Config::Instance()->IsUseDTex() && CanUseDTex()) 
	{
		gum::DTex* dtex = gum::DTex::Instance();
		dtex->LoadSymStart();
		sm::vec2 sz = m_s2_tex->GetSize();
		gum::UID uid = gum::ResourceUID::RawFile(GetFilepath());
		sm::i16_rect r;
		r.xmin = r.ymin = 0;
		r.xmax = sz.x;
		r.ymax = sz.y;
		dtex->LoadSymbol(uid, m_s2_tex->GetTexID(), sz.x, sz.y, r, 0, 1, 0);
		dtex->LoadSymFinish();
	}

	return true;
}

int Image::GetFormat() const
{
	return m_tex->GetFormat();
}

unsigned int Image::GetTexID() const 
{ 
	return m_tex->GetTexID(); 
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
		sm::vec2 pos = mt * (vertices[i] + m_offset);
		if (pos.x < xmin) xmin = pos.x;
		if (pos.x > xmax) xmax = pos.x;
		if (pos.y < ymin) ymin = pos.y;
		if (pos.y > ymax) ymax = pos.y;
	}
	ScreenCache::Instance()->InvalidRect(xmin, ymin, xmax, ymax);
#endif // OPEN_SCREEN_CACHE
}

void Image::QueryTexcoords(float* texcoords, int* texid) const
{
	const float* c2_texcoords = NULL;
	if (Config::Instance()->IsUseDTex() && CanUseDTex()) {
		gum::UID uid = gum::ResourceUID::RawFile(GetFilepath());
		c2_texcoords = gum::DTex::Instance()->QuerySymbol(uid, texid);
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
		texcoords[0] = txmin; texcoords[1] = tymin;
		texcoords[2] = txmax; texcoords[3] = tymin;
		texcoords[4] = txmax; texcoords[5] = tymax;
		texcoords[6] = txmin; texcoords[7] = tymax;
	}
}

void Image::LoadWithClip(const std::string& filepath)
{
	ImageData* img_data = ImageDataMgr::Instance()->GetItem(filepath);

	m_ori_sz.x = img_data->GetWidth();
	m_ori_sz.y = img_data->GetHeight();

	if (img_data->GetFormat() != GPF_RGBA) 
	{
		m_clipped_region.xmin = 0;
		m_clipped_region.ymin = 0;
		m_clipped_region.xmax = m_ori_sz.x;
		m_clipped_region.ymax = m_ori_sz.y;
		m_tex->LoadFromMemory(img_data->GetPixelData(), img_data->GetWidth(),
			img_data->GetHeight(), img_data->GetFormat());
		img_data->RemoveReference();
		return;
	}

	pimg::Condense cd(img_data->GetPixelData(), img_data->GetWidth(), img_data->GetHeight());
	pimg::Rect r;
	uint8_t* c_pixels = cd.GetPixels(r);
	if (!c_pixels) {
		m_tex->LoadFromMemory(img_data->GetPixelData(), img_data->GetWidth(),
			img_data->GetHeight(), img_data->GetFormat());
		img_data->RemoveReference();
		return;
	}
	
	int w = img_data->GetWidth(),
		h = img_data->GetHeight();

	img_data->SetContent(c_pixels, r.Width(), r.Height());
	m_tex->LoadFromMemory(img_data->GetPixelData(), img_data->GetWidth(),
		img_data->GetHeight(), img_data->GetFormat());

	m_clipped_region.xmin = r.xmin;
	m_clipped_region.ymin = r.ymin;
	m_clipped_region.xmax = r.xmax;
	m_clipped_region.ymax = r.ymax;

	img_data->RemoveReference();
}

bool Image::CanUseDTex() const
{
	return m_tex->GetWidth() < 512 && m_tex->GetHeight() < 512;
}

}