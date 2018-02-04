#include "ee0/WxBitmap.h"
#include "ee0/CompSymEditor.h"

#include <gimg_typedef.h>
#include <gimg_import.h>
#include <pimg/Condense.h>
#include <pimg/Rect.h>
#include <sprite2/SymType.h>
#include <sprite2/DrawRT.h>
#include <sprite2/Symbol.h>
#include <s2loader/SymbolFile.h>
#include <gum/Config.h>

namespace ee0
{

static const int SMALL_SIZE = 24;

static const float MAX_WIDTH = 150.0f;
static const float SCALE = 0.5f;

WxBitmap::WxBitmap(const std::string& filepath)
{
	LoadFromFile(filepath);
}

bool WxBitmap::LoadFromFile(const std::string& filepath)
{
	bool ret = false;

	int type = s2loader::SymbolFile::Instance()->Type(filepath.c_str());
	if (type == s2::SYM_IMAGE)
	{
		wxImage image;
		LoadWXImage(filepath, image);
		InitBmp(image, true);
		ret = true;
	}

	return ret;
}

void WxBitmap::InitBmp(const wxImage& image, bool need_scale)
{
	{
		float w = image.GetWidth(),
			  h = image.GetHeight();
		float scale = 1;
		if (need_scale) {
			scale = w > (MAX_WIDTH / SCALE) ? (MAX_WIDTH / w) : SCALE; 
		}
		w = std::max(1.0f, w * scale);
		h = std::max(1.0f, h * scale);
		m_bmp_large = std::make_unique<wxBitmap>(image.Scale(w, h));
	}
	{
		float w = image.GetWidth(),
			h = image.GetHeight();
		float scale = (float)SMALL_SIZE / w;
		w = std::max(1.0f, w * scale);
		h = std::max(1.0f, h * scale);
		m_bmp_small = std::make_unique<wxBitmap>(image.Scale(w, h));
	}
}

void WxBitmap::LoadWXImage(const std::string& filepath, wxImage& dst_img)
{
	static bool inited = false;
	if (!inited)
	{
		wxInitAllImageHandlers();
		inited = true;
	}

	int width, height, format;
	uint8_t* pixels = gimg_import(filepath.c_str(), &width, &height, &format);
	if (format == GPF_RGBA8 && gum::Config::Instance()->GetPreMulAlpha()) {
		gimg_pre_mul_alpha(pixels, width, height);
	}
	if (format != GPF_RGBA8) {
		dst_img.LoadFile(filepath.c_str());
		return;
	}

	pimg::Condense cd(pixels, width, height);
	pimg::Rect r = cd.GetRegion();

	wxImage wx_img;
	wx_img.LoadFile(filepath.c_str());

	wxRect wx_rect;
	int h = height;
	wx_rect.SetLeft(r.xmin);
	wx_rect.SetRight(r.xmax - 1);
	wx_rect.SetTop(h - r.ymax);
	wx_rect.SetBottom(h - r.ymin - 1);

	dst_img = wx_img.GetSubImage(wx_rect);
}

}