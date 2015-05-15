#include "ImageSaver.h"
#include "common/Exception.h"

#include <SOIL/stb_image_write.h>
#include <fstream>

namespace d2d
{

void ImageSaver::storeToFile(const uint8_t* pixels, int width, int height,
							 int channels, const std::string& filename, Type type)
{
	if (!pixels) {
		throw Exception("Fail to store image %s: null pixles!", filename.c_str());
	}

	switch (type)
	{
	case e_png:
		storePNG(pixels, width, height, channels, filename);
		break;
	case e_ppm:
		storePPM(pixels, width, height, channels, filename);
		break;
	case e_gif:
		break;
	}
}

void ImageSaver::storePNG(const uint8_t* pixels, int width, int height,
						  int channels, const std::string& filename)
{
	std::string filepath;
	if (filename.rfind('.') != std::string::npos) {
		filepath = filename;
	} else {
		filepath = filename + ".png";
	}
	stbi_write_png(filepath.c_str(), width, height, channels, pixels, 0);
}

void ImageSaver::storePPM(const uint8_t* pixels, int width, int height,
						  int channels, const std::string& filename)
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
			if (a == 0)
				r = g = b = 0;
			ppm_data[ptr_ppm++] = g;
			ppm_data[ptr_ppm++] = b;
			ppm_data[ptr_ppm++] = r;
			pgm_data[ptr_pgm++] = a;
		}
	}

	// write to file
	std::string ppm_path = filename + ".ppm";
	std::locale::global(std::locale(""));
	std::ofstream ppm_fout(ppm_path.c_str(), std::ios::binary);
	std::locale::global(std::locale("C"));	
//	assert(!ppm_fout.fail());
	ppm_fout.write(reinterpret_cast<const char*>(ppm_data), ppm_size);
	delete[] ppm_data;
	ppm_fout.close();

	std::string pgm_path = filename + ".pgm";
	std::locale::global(std::locale(""));
	std::ofstream pgm_fout(pgm_path.c_str(), std::ios::binary);
	std::locale::global(std::locale("C"));	
//	assert(!pgm_fout.fail());
	pgm_fout.write(reinterpret_cast<const char*>(pgm_data), pgm_size);
	delete[] pgm_data;
	pgm_fout.close();
}

}