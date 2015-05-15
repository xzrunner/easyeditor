#ifndef _EASYIMAGE_LIBPNG_ADAPTER_H_
#define _EASYIMAGE_LIBPNG_ADAPTER_H_

#include <drag2d.h>

namespace eimage
{

class LibpngAdapter
{
public:
	static int Write(const uint8_t* pixels, int width, int height,
		const char* filepath);

	static uint8_t* Read(const char* filepath, int& width, int& height, 
		int& channels, int& format);

	static void ReadHeader(const char* filepath, int& width, int& height);

}; // LibpngAdapter

}

#endif // _EASYIMAGE_LIBPNG_ADAPTER_H_