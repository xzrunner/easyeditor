#ifndef _EASYIMAGE_IMAGE_PACK_H_
#define _EASYIMAGE_IMAGE_PACK_H_

#include <drag2d.h>

namespace eimage
{

class ImagePack
{
public:
	ImagePack(int width, int height);
	~ImagePack();

	enum PackType
	{
		PT_NORMAL,
		PT_CLOCKWISE,
		PT_ANTICLOCKWISE
	};
	void AddImage(const uint8_t* src_buf, int src_w, int src_h, int dst_x, int dst_y, 
		PackType type = PT_NORMAL, bool bpp4 = true);

	void AddImage(const d2d::Image* img, int x, int y, int w, int h, bool clockwise, bool bpp4 = true);

	void OutputToFile(const wxString& filepath) const;

	uint8_t* GetPixels() const { return m_pixels; }

private:
	uint8_t* m_pixels;

	int m_width, m_height;

}; // ImagePack

}

#endif // _EASYIMAGE_IMAGE_PACK_H_