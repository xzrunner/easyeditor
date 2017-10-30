#ifndef _EASYEDITOR_BITMAP_H_
#define _EASYEDITOR_BITMAP_H_

#include <wx/bitmap.h>

#include <string>
#include <memory>

#include <stdint.h>

namespace ee
{

class Symbol;

class Bitmap
{
public:
	Bitmap(const std::string& filepath);
	Bitmap(const std::string& filepath, uint8_t* pixels, int w, int h, int fmt);
	Bitmap(const Symbol& sym);
	virtual ~Bitmap();

	const wxBitmap* GetLargeBmp() const { return m_bmp_large; }
	const wxBitmap* GetSmallBmp() const { return m_bmp_small; }

private:
	bool LoadFromFile(const std::string& filepath);
	void LoadFromData(uint8_t* pixels, int w, int h, int fmt);
	void LoadFromSym(const Symbol& sym);

	void InitBmp(const wxImage& image, bool scale);

	static void GetImage(const std::string& filepath, wxImage& image);

private:
	std::string m_filepath;

	wxBitmap* m_bmp_large;
	wxBitmap* m_bmp_small;

}; // Bitmap

using BitmapPtr = std::shared_ptr<Bitmap>;

}

#endif // _EASYEDITOR_BITMAP_H_