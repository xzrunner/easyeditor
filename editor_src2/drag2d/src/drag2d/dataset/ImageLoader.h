#pragma once

namespace d2d
{
	class ImageLoader
	{
	public:
		static unsigned char* loadTexture(void* data, int& width, int& height, unsigned int& texture, int& format);
		static unsigned char* loadTexture(const std::string& filepath, int& width, int& height, unsigned int& texture, int& format);

	private:
		static unsigned char* loadData(const std::string& filepath, int& width, int& height, int& format);

		static void loadTexture(unsigned int& texture, unsigned char* pixel, int format, int width, int height);

		static unsigned char* loadPNG(void* data, int& width, int& height, int& format);
 		static unsigned char* loadPPM(const std::string& filename, int& width, int& height, int& format);
 		static unsigned char* loadPPM(const std::string& filename, int& width, int& height);
 		static unsigned char* loadPGM(const std::string& filename, int& width, int& height);

	}; // ImageLoader
}