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

	void AddImage(const d2d::Image* img, int x, int y, int w, int h);

	void OutputToFile(const wxString& filepath) const;

private:
	byte* m_pixels;

	int m_width, m_height;

}; // ImagePack

}

#endif // _EASYIMAGE_IMAGE_PACK_H_