#ifndef _DRAG2D_LIBPNG_ADAPTER_H_
#define _DRAG2D_LIBPNG_ADAPTER_H_

#include <stdint.h>
#include <string>

namespace d2d
{

class LibpngAdapter
{
public:
	static int Write(const uint8_t* pixels, int width, int height,
		const std::string& filename);

	static uint8_t* Read(const std::string& filename, int& width, int& height, 
		int& channels, int& format);

	static void ReadHeader(const std::string& filename, int& width, int& height);

}; // LibpngAdapter

}

#endif // _DRAG2D_LIBPNG_ADAPTER_H_