#ifndef _EASYIMAGE_IMAGE_PROCESSOR_H_
#define _EASYIMAGE_IMAGE_PROCESSOR_H_

namespace d2d
{
	class Image;
	class Rect;
}

namespace eimage
{

class ImageProcessor
{
public:
	ImageProcessor(const d2d::Image* image);

	const unsigned char* clip(int xmin, int xmax, int ymin, int ymax) const;

	// remove transparent border
	// return: make to center
	d2d::Rect trim() const;

private:
	bool isTransparent(int x, int y) const;

private:
	const d2d::Image* m_image;

}; // ImageProcessor

}

#endif // _EASYIMAGE_IMAGE_PROCESSOR_H_
