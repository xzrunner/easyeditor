#pragma once

#include <stdlib.h>

namespace d2d
{
	class LibJpeg
	{
	public:
		struct ImageData 
		{
			unsigned char* pixels;
			int width;
			int height;
			int channels;

			ImageData() : pixels(NULL), width(0), height(0), channels(0) {}
		};

	public:
		static void write_JPEG_file(const char* filename, int quality, const ImageData& data);
		static int read_JPEG_file(const char* filename, ImageData& data);

	}; // LibJpeg
}

