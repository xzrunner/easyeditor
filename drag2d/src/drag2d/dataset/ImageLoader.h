#ifndef _DRAG2D_IMAGE_LOADER_H_
#define _DRAG2D_IMAGE_LOADER_H_

#include <stdint.h>
#include <string>

namespace d2d
{

class ImageLoader
{
public:
	static uint8_t* FileToTexture(const std::string& filepath, int& width, int& height, unsigned int& texture, int& channels);

	static uint8_t* FileToPixels(const std::string& filepath, int& width, int& height, int& channels, int& format);

	static void PixelsToTexture(unsigned int& texture, const uint8_t* pixel, int width, int height, int channels, int format);

private:
	// alpha为0时，把rgb也设为0
	// 否则用shader画的时候有奇怪的透明部分；导出ppm格式时透明的部分不对
	// 或设成255
	static void FormatPixelsAlpha(uint8_t* pixels, int width, int height, int val);

	static void PreMuiltiAlpha(uint8_t* pixels, int width, int height);

	static void RemoveGhostPixel(uint8_t* pixels, int width, int height);

}; // ImageLoader

}

#endif // _DRAG2D_IMAGE_LOADER_H_