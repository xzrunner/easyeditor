#pragma once

#include <string>
#include <drag2d.h>

namespace cocextract
{
	class ImageHelper
	{
	public:
		ImageHelper();
		~ImageHelper();

		const unsigned char* clip(int xmin, int xmax, int ymin, int ymax);

		void writeToFile(const unsigned char* pixels, int width, int height,
			const std::string& filename);

		void loadPNG(const std::string& filename);
		void loadPPM(const std::string& filename);

	private:
		void load();

		unsigned char* loadPPM(const std::string& filename, int& width, int& height);
		unsigned char* loadPGM(const std::string& filename, int& width, int& height);

	private:
		std::string m_filename;

		int m_width, m_height;
		int m_format;
		unsigned char* m_pixels;

	}; // ImageExtract
}