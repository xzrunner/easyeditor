#ifndef _EASYIMAGE_PIXEL_AREA_ARRAY_H_
#define _EASYIMAGE_PIXEL_AREA_ARRAY_H_

#include <drag2d.h>

namespace eimage
{

class PixelAreaArray
{
public:
	PixelAreaArray(const bool* pixels, int width, int height, bool to_block4);
	~PixelAreaArray();

	int GetRectArea(int x, int y, int w, int h) const;
	void CutByRect(int x, int y, int w, int h, int& left_area);

private:
	void FixPixelsToBlock4();

	void LoadFromPixels();

private:
	bool* m_pixels;

	int* m_area;

	int m_width, m_height;

}; // PixelAreaArray

}

#endif // _EASYIMAGE_PIXEL_AREA_ARRAY_H_