#ifndef _DRAG2D_IMAGE_SAVER_H_
#define _DRAG2D_IMAGE_SAVER_H_

#include <stdint.h>
#include <string>

namespace d2d
{

class ImageSaver
{
public:
	enum Type
	{
		e_png,
		e_ppm,
		e_gif
	};

	static void StoreToFile(const uint8_t* pixels, int width, int height,
		int channels, const std::string& filename, Type type);

}; // ImageSaver

}

#endif // _DRAG2D_IMAGE_SAVER_H_