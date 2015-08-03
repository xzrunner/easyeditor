#pragma once

#include <stdint.h>
#include <string>

namespace d2d
{

class ImageSaver
{
public:
	enum Type
	{
		e_png,
		e_ppm,
		e_gif
	};

	static void storeToFile(const uint8_t* pixels, int width, int height,
		int channels, const std::string& filename, Type type);

private:
	static int storePNG(const uint8_t* pixels, int width, int height,
		int channels, const std::string& filename);
	static void storePPM(const uint8_t* pixels, int width, int height,
		int channels, const std::string& filename);

}; // ImageSaver

}