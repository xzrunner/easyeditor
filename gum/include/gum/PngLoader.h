#ifndef _GUM_PNG_LOADER_H_
#define _GUM_PNG_LOADER_H_

#include <string>
#include <stdint.h>

#include <render/render.h>

namespace gum
{

class PngLoader
{
public:
	static int Write(const uint8_t* pixels, int width, int height, const std::string& filename);

	static uint8_t* Read(const std::string& filename, int& width, int& height, TEXTURE_FORMAT& format);

	static void ReadHeader(const std::string& filename, int& width, int& height);

}; // PngLoader

}

#endif // _GUM_PNG_LOADER_H_
