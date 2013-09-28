#include "Bitmap.h"

namespace d2d
{

Bitmap::Bitmap()
	: m_bitmap(NULL)
	, m_scale(1.0f)
{
}

Bitmap::Bitmap(wxBitmap* bitmap)
	: m_bitmap(bitmap)
	, m_scale(1.0f)
{
}

Bitmap::~Bitmap()
{
	delete m_bitmap;
}

bool Bitmap::loadFromFile(const wxString& filepath)
{
	static bool init = false;
	if (!init)
	{
		wxInitAllImageHandlers();
//		wxImage::AddHandler(new wxPNGHandler);
//		wxImage::AddHandler(new wxJPEGHandler);
	//	wxImage::AddHandler(new wxBMPHandler);
		init = true;
	}

 	wxImage image;
 	image.LoadFile(filepath);
 
	m_scale = computeScale(image.GetWidth());
 	wxImage scaled = image.Scale(image.GetWidth() * m_scale, image.GetHeight() * m_scale);
 	m_bitmap = new wxBitmap(scaled);

	return true;
}

float Bitmap::computeScale(float width)
{
	return width > 300 ? 150.0f / width : 0.5f;
}

} // d2d