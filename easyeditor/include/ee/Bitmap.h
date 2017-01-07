#ifndef _EASYEDITOR_BITMAP_H_
#define _EASYEDITOR_BITMAP_H_

#include <CU_RefCountObj.h>

#include <wx/bitmap.h>

#include <stdint.h>

namespace ee
{

class Symbol;

class Bitmap : public cu::RefCountObj
{
public:
	Bitmap(const std::string& filepath);
	Bitmap(const std::string& filepath, uint8_t* pixels, int w, int h, int fmt);
	Bitmap(const Symbol* sym);
	virtual ~Bitmap();

	const wxBitmap* GetLargeBmp() const { return m_bmp_large; }
	const wxBitmap* GetSmallBmp() const { return m_bmp_small; }

private:
	bool LoadFromFile(const std::string& filepath);
	void LoadFromData(uint8_t* pixels, int w, int h, int fmt);
	void LoadFromSym(const Symbol* sym);

	void InitBmp(const wxImage& image, bool scale);

	static unsigned char* TransRGBA2RGB(unsigned char* rgba, int width, int height);

	static void GetImage(const std::string& filepath, wxImage& image);

private:
	std::string m_filepath;

	wxBitmap* m_bmp_large;
	wxBitmap* m_bmp_small;

}; // Bitmap

}

#endif // _EASYEDITOR_BITMAP_H_