#include "Bitmap.h"
#include "Image.h"
#include "ISymbol.h"
#include "SymbolMgr.h"

#include "common/FileNameParser.h"
#include "common/Exception.h"
#include "render/Snapshoot.h"

#include <gl/gl.h>
#include <wx/filename.h>

#include <easyimage.h>

namespace d2d
{

static const int SMALL_SIZE = 24;

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

	if (!wxFileName::FileExists(filepath)) {
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
		InitBmp(image);
	}
	else if (FileNameParser::isType(filepath, FileNameParser::e_image))
	{
		wxImage image;
		GetImage(filepath, image);
		InitBmp(image);
	}
	else if (FileNameParser::isType(filepath, FileNameParser::e_terrain2d))
	{
		;
	}
	else
	{
		ISymbol* symbol = SymbolMgr::Instance()->FetchSymbol(filepath);
		d2d::Rect rect = symbol->GetSize();
		int w = std::max(1.0f, rect.xLength()),
			h = std::max(1.0f, rect.yLength());

 		if (w > 2048 || h > 2048) {
 			return true;
 		}

		d2d::Snapshoot ss(w, h);
		unsigned char* rgba = ss.OutputToMemory(symbol, true);
		unsigned char* rgb = TransRGBA2RGB(rgba, w, h);
		delete[] rgba;

		wxImage image(w, h, rgb, true);
		InitBmp(image);
		delete[] rgb;
		symbol->Release();
	}

	return true;
}

void Bitmap::InitBmp(const wxImage& image)
{
	int width = image.GetWidth();
	{
		if (m_bmp_large) {
			delete m_bmp_large;
		}
		float scale_large = width > 300 ? 150.0f / width : 0.5f;
		int w = std::max(1.0f, image.GetWidth() * scale_large);
		int h = std::max(1.0f, image.GetHeight() * scale_large);
		m_bmp_large = new wxBitmap(image.Scale(w, h));
	}
	{
		if (m_bmp_small) {
			delete m_bmp_small;
		}
		float scale_small = (float)SMALL_SIZE / width;
		int w = std::max(1.0f, image.GetWidth() * scale_small);
		int h = std::max(1.0f, image.GetHeight() * scale_small);
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
	eimage::ImageTrim trim(*img_data);
	Rect trim_r = trim.Trim();
	img_data->Release();

	if (trim_r.isValid()) {
		wxImage wx_img;
		wx_img.LoadFile(filepath);

		wxRect wx_rect;
		wx_rect.SetLeft(trim_r.xMin);
		wx_rect.SetRight(trim_r.xMax - 1);
		wx_rect.SetTop(h - trim_r.yMax);
		wx_rect.SetBottom(h - trim_r.yMin - 1);

		dst_img = wx_img.GetSubImage(wx_rect);
	} else {
		dst_img.LoadFile(filepath);
	}
}

} // d2d