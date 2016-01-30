#ifndef _EASYEDITOR_LIBJPEG_ADAPTER_H_
#define _EASYEDITOR_LIBJPEG_ADAPTER_H_

#include <stdint.h>

namespace ee
{

class LibjpegAdapter
{
public:
	static void Write(const uint8_t* pixels, int width, int height,
		const char* filename, int quality);
	static uint8_t* Read(const char* filename, int& width, int& height, 
		int& channels, int& format);

}; // LibjpegAdapter

}

#endif // _EASYEDITOR_LIBJPEG_ADAPTER_H_