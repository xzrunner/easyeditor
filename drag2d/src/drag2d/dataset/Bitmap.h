#ifndef _DRAG2D_BITMAP_H_
#define _DRAG2D_BITMAP_H_

#include "common/ResourcesMgr.h"
#include "common/Object.h"

#include <wx/bitmap.h>

namespace d2d
{

class Bitmap;
typedef ResourcesMgr<Bitmap> BitmapMgr;

class Bitmap : public Object
{
public:
	Bitmap();
	virtual ~Bitmap();

	bool LoadFromFile(const std::string& filepath);

	const wxBitmap* GetLargeBmp() const { return m_bmp_large; }
	const wxBitmap* GetSmallBmp() const { return m_bmp_small; }

private:
	void InitBmp(const wxImage& image);

	static unsigned char* TransRGBA2RGB(unsigned char* rgba, int width, int height);

	static void GetImage(const std::string& filepath, wxImage& image);

private:
	std::string m_filename;

	wxBitmap* m_bmp_large;
	wxBitmap* m_bmp_small;

	float m_scale;

}; // Bitmap

}

#endif // _DRAG2D_BITMAP_H_