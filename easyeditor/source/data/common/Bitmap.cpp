#include "Bitmap.h"
#include "Config.h"
#include "SettingData.h"
#include "ImageData.h"
#include "Symbol.h"
#include "SymbolMgr.h"
#include "SymbolFile.h"
#include "FileHelper.h"
#include "Exception.h"
#include "SymbolType.h"

#include <gimg_typedef.h>
#include <gimg_import.h>
#include <pimg/Condense.h>
#include <pimg/Rect.h>
#include <sprite2/SymType.h>
#include <sprite2/DrawRT.h>

namespace ee
{

static const int SMALL_SIZE = 24;

static const float MAX_WIDTH = 150.0f;
static const float SCALE = 0.5f;

Bitmap::Bitmap(const std::string& filepath)
	: m_filepath(filepath)
	, m_bmp_large(NULL)
	, m_bmp_small(NULL)
{
	LoadFromFile(filepath);
}

Bitmap::Bitmap(const std::string& filepath, uint8_t* pixels, int w, int h, int fmt)
	: m_filepath(filepath)
	, m_bmp_large(NULL)
	, m_bmp_small(NULL)
{
	LoadFromData(pixels, w, h, fmt);
}

Bitmap::Bitmap(const Symbol* sym)
	: m_filepath(sym->GetFilepath())
	, m_bmp_large(NULL)
	, m_bmp_small(NULL)
{
	LoadFromSym(sym);
}

Bitmap::~Bitmap()
{
	delete m_bmp_large;
	delete m_bmp_small;
}

bool Bitmap::LoadFromFile(const std::string& filepath)
{
	m_filepath = filepath;

	if (!Config::Instance()->GetSettings().load_image) {
		return true;
	}

	int type = ee::SymbolFile::Instance()->Type(filepath);
	if (filepath.find("pvr") != std::string::npos || filepath.find("pkm") != std::string::npos) 
	{
		ImageData* img_data = ImageDataMgr::Instance()->GetItem(filepath);

		int w = img_data->GetWidth(),
			h = img_data->GetHeight();
		const uint8_t* data = img_data->GetPixelData();

		uint8_t* rgb_data = new uint8_t[w * h * 3];
		const uint8_t* src = data;
		uint8_t* dst = rgb_data;
		for (int i = 0, n = w * h; i < n; ++i) {
			memcpy(dst, src, 3);
			src += 4;
			dst += 3;
		}

		wxImage image(w, h, rgb_data, true);
		InitBmp(image, true);

		delete rgb_data;
	}
	else if (type == s2::SYM_IMAGE)
	{
		wxImage image;
		GetImage(filepath, image);
		InitBmp(image, true);
	}
	else if (type == ee::SYM_TERRAIN2D)
	{
		;
	}
	else
	{
		if (filepath == SYM_GROUP_TAG) {
			return false;
		} else {
			Symbol* sym = SymbolMgr::Instance()->FetchSymbol(filepath);
			LoadFromSym(sym);
			sym->RemoveReference();
		}
	}

	return true;
}

void Bitmap::LoadFromData(uint8_t* pixels, int w, int h, int fmt)
{
	switch (fmt)
	{
	case GPF_RGB:
		InitBmp(wxImage(w, h, (unsigned char*)pixels, true), false);
		break;
	case GPF_RGBA:
		{
			uint8_t* rgb = gimg_rgba2rgb(pixels, w, h);
			gimg_revert_y(rgb, w, h, GPF_RGB);
			InitBmp(wxImage(w, h, rgb, true), false);
			free(rgb);
		}
		break;
	}
}

void Bitmap::LoadFromSym(const Symbol* sym)
{
	sm::rect rect = sym->GetBounding();
	if (!rect.IsValid()) {
		return;
	}

	float w = std::max(1.0f, rect.Size().x),
		  h = std::max(1.0f, rect.Size().y);
	float scale = w > (MAX_WIDTH / SCALE) ? (MAX_WIDTH / w) : SCALE; 
	w *= scale;
	h *= scale;
	w = std::max(1.0f, w);
	h = std::max(1.0f, h);

 	s2::DrawRT rt;
 	rt.Draw(sym, true, scale);
 	unsigned char* rgba = rt.StoreToMemory(w, h, 4);
 	if (!rgba) {
 		return;
 	}
 
 	uint8_t* rgb = gimg_rgba2rgb(rgba, w, h);
 
 	wxImage image(w, h, rgb, true);
 	InitBmp(image, false);
 
 	delete[] rgb;
 	delete[] rgba;
}

void Bitmap::InitBmp(const wxImage& image, bool need_scale)
{
	{
		if (m_bmp_large) {
			delete m_bmp_large;
		}
		float w = image.GetWidth(),
			  h = image.GetHeight();
		float scale = 1;
		if (need_scale) {
			scale = w > (MAX_WIDTH / SCALE) ? (MAX_WIDTH / w) : SCALE; 
		}
		w = std::max(1.0f, w * scale);
		h = std::max(1.0f, h * scale);
		m_bmp_large = new wxBitmap(image.Scale(w, h));
	}
	{
		if (m_bmp_small) {
			delete m_bmp_small;
		}
		float w = image.GetWidth(),
			h = image.GetHeight();
		float scale = (float)SMALL_SIZE / w;
		w = std::max(1.0f, w * scale);
		h = std::max(1.0f, h * scale);
		m_bmp_small = new wxBitmap(image.Scale(w, h));
	}
}

void Bitmap::GetImage(const std::string& filepath, wxImage& dst_img)
{
	static bool inited = false;
	if (!inited)
	{
		wxInitAllImageHandlers();
		inited = true;
	}

	ImageData* img = ImageDataMgr::Instance()->GetItem(filepath);
	if (img->GetFormat() != GPF_RGBA) {
		img->RemoveReference();
		dst_img.LoadFile(filepath);
		return;
	}

	pimg::Condense cd(img->GetPixelData(), img->GetWidth(), img->GetHeight());
	pimg::Rect r = cd.GetRegion();
	img->RemoveReference();

	wxImage wx_img;
	wx_img.LoadFile(filepath);

	wxRect wx_rect;
	int h = img->GetHeight();
	wx_rect.SetLeft(r.xmin);
	wx_rect.SetRight(r.xmax - 1);
	wx_rect.SetTop(h - r.ymax);
	wx_rect.SetBottom(h - r.ymin - 1);

	dst_img = wx_img.GetSubImage(wx_rect);
}

}