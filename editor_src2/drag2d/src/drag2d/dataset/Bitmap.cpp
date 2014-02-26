#include "Bitmap.h"
#include "Image.h"

namespace d2d
{

Bitmap::Bitmap()
	: m_bitmap(NULL)
{
}

Bitmap::Bitmap(wxBitmap* bitmap)
	: m_bitmap(bitmap)
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
		init = true;
	}

	wxImage image;
	getImage(filepath, image);

	m_bitmap = getBitmap(image);

	return true;
}

void Bitmap::getImage(const wxString& filepath, wxImage& image)
{
	wxImage totimg;
	totimg.LoadFile(filepath);

	Image* pImage = ImageMgr::Instance()->getItem(filepath);
	Rect rect = pImage->getRegion();
	float w = totimg.GetWidth();
	float h = totimg.GetHeight();
	// invert y
	float offset = rect.yCenter() * 2;
	rect.translate(Vector(w*0.5f, h*0.5f));

	wxRect wx_rect;
	wx_rect.SetLeft(rect.xMin);
	wx_rect.SetRight(rect.xMax - 1);
	wx_rect.SetTop(rect.yMin - offset);
	wx_rect.SetBottom(rect.yMax - offset - 1);

	image = totimg.GetSubImage(wx_rect);
}

wxBitmap* Bitmap::getBitmap(const wxImage& image)
{
	wxBitmap* bmp = NULL;

	float scale = computeScale(image.GetWidth());

	int w = image.GetWidth() * scale;
	int h = image.GetHeight() * scale;
	if (w > 1 && h > 1)
		bmp = new wxBitmap(image.Scale(w, h));
	else
		bmp = new wxBitmap(image.Scale(image.GetWidth(), image.GetHeight()));
	return bmp;
}

float Bitmap::computeScale(float width)
{
	return width > 300 ? 150.0f / width : 0.5f;
}

} // d2d