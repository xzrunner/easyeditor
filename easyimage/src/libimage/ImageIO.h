#ifndef _EASYIMAGE_IMAGE_IO_H_
#define _EASYIMAGE_IMAGE_IO_H_

#include <drag2d.h>

namespace eimage
{

class ImageIO
{
public:
	
	static int Write(const uint8_t* pixels, int width, int height,
		const char* filepath);

	static uint8_t* Read(const char* filepath, int& width, int& height, 
		int& channels, int& format);

	static void ReadHeader(const char* filepath, int& width, int& height);

}; // ImageIO

}

#endif // _EASYIMAGE_IMAGE_IO_H_