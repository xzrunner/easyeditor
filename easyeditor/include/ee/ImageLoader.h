#ifndef _EASYEDITOR_IMAGE_LOADER_H_
#define _EASYEDITOR_IMAGE_LOADER_H_

#include <string>

#include <stdint.h>

namespace ee
{

class ImageLoader
{
public:
	static uint8_t* FileToTexture(const std::string& filepath, int& width, int& height, unsigned int& texture, int& channels);

	static uint8_t* FileToPixels(const std::string& filepath, int& width, int& height, int& channels, int& format);

	static void PixelsToTexture(unsigned int& texture, const uint8_t* pixel, int width, int height, int channels, int format);

private:
	static void FormatPixelsAlpha(uint8_t* pixels, int width, int height, int val);

	static void PreMuiltiAlpha(uint8_t* pixels, int width, int height);

	static void RemoveGhostPixel(uint8_t* pixels, int width, int height);

}; // ImageLoader

}

#endif // _EASYEDITOR_IMAGE_LOADER_H_