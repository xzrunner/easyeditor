#include "Bitmap.h"
#include "Config.h"
#include "SettingData.h"
#include "ImageData.h"
//#include "Image.h"
#include "Symbol.h"
#include "SymbolMgr.h"
#include "FileType.h"
#include "FileHelper.h"
#include "Exception.h"
#include "Snapshoot.h"
#include "ImageTrim.h"

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
	if (!Config::Instance()->GetSettings().load_image) {
		return true;
	}

	if (!FileHelper::IsFileExist(filepath)) {
		throw Exception("File: %s don't exist!", filepath.c_str());
	}

	m_filename = filepath;
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

	if (filepath.find("pvr") != std::string::npos) 
	{
		ImageData* img_data = ImageDataMgr::Instance()->GetItem(filepath);
		wxImage image(img_data->GetWidth(), img_data->GetHeight(), (unsigned char*)(img_data->GetPixelData()), true);
//		image.SetData((unsigned char*)(img_data->GetPixelData()), img_data->GetWidth(), img_data->GetHeight());
		InitBmp(image, true);
	}
	else if (FileType::IsType(filepath, FileType::e_image))
	{
		wxImage image;
		GetImage(filepath, image);
		InitBmp(image, true);
	}
	else if (FileType::IsType(filepath, FileType::e_terrain2d))
	{
		;
	}
	else
	{
		Symbol* symbol = SymbolMgr::Instance()->FetchSymbol(filepath);
		Rect rect = symbol->GetSize();
		float w = std::max(1.0f, rect.Width()),
			h = std::max(1.0f, rect.Height());
		float scale = w > (MAX_WIDTH / SCALE) ? (MAX_WIDTH / w) : SCALE; 
		w *= scale;
		h *= scale;

		Snapshoot ss(w, h);
		unsigned char* rgba = ss.OutputToMemory(symbol, true, scale);
		unsigned char* rgb = TransRGBA2RGB(rgba, w, h);
		delete[] rgba;

		wxImage image(w, h, rgb, true);
		InitBmp(image, false);
		delete[] rgb;
		symbol->Release();
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
	Rect trim_r = trim.Trim();
	img_data->Release();

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