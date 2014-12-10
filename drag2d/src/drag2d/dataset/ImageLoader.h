#pragma once

#include "common/tools.h"

namespace d2d
{

class ImageLoader
{
public:
	static byte* loadTexture(const std::string& filepath, int& width, int& height, unsigned int& texture, int& channels);

	static byte* loadData(const std::string& filepath, int& width, int& height, int& channels, int& format);

	// alpha为0时，把rgb也设为0
	// 否则用shader画的时候有奇怪的透明部分；导出ppm格式时透明的部分不对
	static void checkPixelsDataAlpha(byte* pixels, int width, int height);

	static void loadTexture(unsigned int& texture, const byte* pixel, int width, int height, int channels, int format);

private:
	static byte* loadImageByLibpng(const std::string& filename, int& width, int& height, int& channels, int& format);
	static byte* loadImageBySOIL(const std::string& filename, int& width, int& height, int& channels);

	static byte* loadPNM(const std::string& filename, int& width, int& height);
	static byte* loadPPM(const std::string& filename, int& width, int& height);
	static byte* loadPGM(const std::string& filename, int& width, int& height);

}; // ImageLoader

}