#ifndef _EASYEDITOR_IMAGE_SAVER_H_
#define _EASYEDITOR_IMAGE_SAVER_H_

#include <string>

#include <stdint.h>

namespace ee
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
		int channels, const std::string& filename, Type type, bool reverse = true);

}; // ImageSaver

}

#endif // _EASYEDITOR_IMAGE_SAVER_H_