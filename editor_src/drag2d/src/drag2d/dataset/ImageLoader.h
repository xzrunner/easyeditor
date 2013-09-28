#pragma once

namespace d2d
{
	class ImageLoader
	{
	public:
		static void loadTexture(void* data, int& width, int& height, unsigned int& texture);

		static unsigned char* loadData(const char* filepath, int& width, int& height, int& format);

	private:
		static unsigned char* loadPNG(void* data, int& width, int& height, int& format);

	}; // ImageLoader
}