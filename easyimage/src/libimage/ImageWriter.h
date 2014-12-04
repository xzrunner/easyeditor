#ifndef _EASYIMAGE_IMAGE_WRITER_H_
#define _EASYIMAGE_IMAGE_WRITER_H_

#include <drag2d.h>

namespace eimage
{

class ImageWriter
{
public:
	
	static int Write(const byte* pixels, int width, int height,
		const char* filepath);

}; // ImageWriter

}

#endif // _EASYIMAGE_IMAGE_WRITER_H_