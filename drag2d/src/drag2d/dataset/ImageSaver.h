#pragma once

#include "common/tools.h"

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
		const std::string& filename, Type type);

private:
	static void storePNG(const uint8_t* pixels, int width, int height,
		const std::string& filename);
	static void storePPM(const uint8_t* pixels, int width, int height,
		const std::string& filename);

}; // ImageSaver

}