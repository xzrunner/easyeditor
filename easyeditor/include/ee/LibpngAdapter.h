#ifndef _EASYEDITOR_LIBPNG_ADAPTER_H_
#define _EASYEDITOR_LIBPNG_ADAPTER_H_

#include <stdint.h>

namespace ee
{

class LibpngAdapter
{
public:
	static int Write(const uint8_t* pixels, int width, int height, int channels,
		const char* filename, bool reverse = true);

	static uint8_t* Read(const char* filename, int& width, int& height, 
		int& channels, int& format);

	static void ReadHeader(const char* filename, int& width, int& height);

}; // LibpngAdapter

}

#endif // _EASYEDITOR_LIBPNG_ADAPTER_H_