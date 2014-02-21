#include "ImageSaver.h"

#include <SOIL/stb_image_write.h>

namespace d2d
{

void ImageSaver::storeToFile(const uint8_t* pixels, int width, int height,
	const std::string& filename, Type type)
{
	switch (type)
	{
	case e_png:
		storePNG(pixels, width, height, filename);
		break;
	case e_ppm:
		storePPM(pixels, width, height, filename);
		break;
	}
}

void ImageSaver::storePNG(const uint8_t* pixels, int width, int height,
	const std::string& filename)
{
	std::string filepath = filename + ".png";
	stbi_write_png(filepath.c_str(), width, height, 4, pixels, 0);
}

void ImageSaver::storePPM(const uint8_t* pixels, int width, int height,
	const std::string& filename)
{
	// ppm
	int ppm_size = 16 + width * height * 3;
	unsigned char* ppm_data = new unsigned char[ppm_size];
	sprintf((char*)ppm_data, 
		"P6\n"
		"%d %d\n"
		"%d\n"
		, width, height, 255);

	// pgm
	int pgm_size = 16 + width * height;
	unsigned char* pgm_data = new unsigned char[pgm_size];
	sprintf((char*)pgm_data, 
		"P5\n"
		"%d %d\n"
		"%d\n"
		, width, height, 255);

	int ptr_ppm = 16, ptr_pgm = 16;
	int ptr_src = 0;
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			uint8_t r = pixels[ptr_src++],
				    g = pixels[ptr_src++],
					b = pixels[ptr_src++],
					a = pixels[ptr_src++];
			ppm_data[ptr_ppm++] = g;
			ppm_data[ptr_ppm++] = b;
			ppm_data[ptr_ppm++] = r;
			pgm_data[ptr_pgm++] = a;
		}
	}

	// write to file
	std::string ppm_path = filename + ".ppm";
	std::ofstream ppm_fout(ppm_path.c_str(), std::ios::binary);
	assert(!ppm_fout.fail());
	ppm_fout.write(reinterpret_cast<const char*>(ppm_data), ppm_size);
	delete[] ppm_data;
	ppm_fout.close();

	std::string pgm_path = filename + ".pgm";
	std::ofstream pgm_fout(pgm_path.c_str(), std::ios::binary);
	assert(!pgm_fout.fail());
	pgm_fout.write(reinterpret_cast<const char*>(pgm_data), pgm_size);
	delete[] pgm_data;
	pgm_fout.close();
}

}