#include "Bitmap.h"
#include "Config.h"
#include "SettingData.h"
#include "ImageData.h"
//#include "Image.h"
#include "Symbol.h"
#include "SymbolMgr.h"
#include "SymbolFile.h"
#include "FileHelper.h"
#include "Exception.h"
#include "Snapshoot.h"
#include "ImageTrim.h"
#include "SymbolType.h"

#include <sprite2/SymType.h>

#include <gl/glew.h>
//#include <wx/filename.h>

//#include <easyimage.h>

namespace ee
{

static const int SMALL_SIZE = 24;

static const float MAX_WIDTH = 150.0f;
static const float SCALE = 0.5f;

Bitmap::Bitmap()
	: m_bmp_large(NULL)
	, m_bmp_small(NULL)
{
}

Bitmap::~Bitmap()
{
	BitmapMgr::Instance()->RemoveItem(m_filename);
	delete m_bmp_large;
}

bool Bitmap::LoadFromFile(const std::string& filepath)
{
	m_filename = filepath;

	if (!Config::Instance()->GetSettings().load_image) {
		return true;
	}

	if (!FileHelper::IsFileExist(filepath)) {
		throw Exception("File: %s don't exist!", filepath.c_str());
	}

	const GLubyte* test = glGetString(GL_VERSION);
	if (!test) {
		return true;
	}

	static bool inited = false;
	if (!inited)
	{
		wxInitAllImageHandlers();
		inited = true;
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
		Symbol* sym = SymbolMgr::Instance()->FetchSymbol(filepath);
		sm::rect rect = sym->GetBounding();
		float w = std::max(1.0f, rect.Size().x),
			  h = std::max(1.0f, rect.Size().y);
		float scale = w > (MAX_WIDTH / SCALE) ? (MAX_WIDTH / w) : SCALE; 
		w *= scale;
		h *= scale;
		w = std::max(1.0f, w);
		h = std::max(1.0f, h);

		Snapshoot ss(w, h);
		unsigned char* rgba = ss.OutputToMemory(sym, true, scale);
		unsigned char* rgb = TransRGBA2RGB(rgba, w, h);
		delete[] rgba;

		wxImage image(w, h, rgb, true);
		InitBmp(image, false);
		delete[] rgb;
		sym->RemoveReference();
	}

	return true;
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

unsigned char* Bitmap::TransRGBA2RGB(unsigned char* rgba, int width, int height)
{
	unsigned char* rgb = new unsigned char[width*height*3];
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			int src = (i*width+j)*4;
			int dst = (i*width+j)*3;
			memcpy(&rgb[dst], &rgba[src], sizeof(unsigned char) * 3);
		}
	}
	return rgb;
}

void Bitmap::GetImage(const std::string& filepath, wxImage& dst_img)
{
	ImageData* img_data = ImageDataMgr::Instance()->GetItem(filepath);
	int h = img_data->GetHeight();
	ImageTrim trim(*img_data);
	sm::rect trim_r = trim.Trim();
	img_data->RemoveReference();

	if (trim_r.IsValid()) {
		wxImage wx_img;
		wx_img.LoadFile(filepath);

		wxRect wx_rect;
		wx_rect.SetLeft(trim_r.xmin);
		wx_rect.SetRight(trim_r.xmax - 1);
		wx_rect.SetTop(h - trim_r.ymax);
		wx_rect.SetBottom(h - trim_r.ymin - 1);

		dst_img = wx_img.GetSubImage(wx_rect);
	} else {
		dst_img.LoadFile(filepath);
	}
}

}