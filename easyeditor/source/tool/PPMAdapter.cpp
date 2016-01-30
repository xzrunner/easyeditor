#include "PPMAdapter.h"

#include <fstream>

#include <assert.h>

namespace ee
{

int PPMAdapter::Write(const uint8_t* pixels, int width, int height, const std::string& filename)
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

	return 0;
}

uint8_t* PPMAdapter::Read(const std::string& filename, int& width, int& height)
{
	return ReadPNM(filename, width, height);
}

uint8_t* PPMAdapter::ReadPNM(const std::string& filename, int& width, int& height)
{
	std::string filepath;
	int w0, h0, w1, h1;
	filepath = filename + ".ppm";
	uint8_t* ppm = ReadPPM(filepath, w0, h0); 
	filepath = filename + ".pgm";
	uint8_t* pgm = ReadPGM(filepath, w1, h1);
	assert(w0 == w1 && h0 == h1);

	width = w0;
	height = h0;
	int size = width * height * 4;
	uint8_t* pixels = new uint8_t[size];

	int ptr_ppm = 0, ptr_pgm = 0, ptr_dst = 0;
	while (ptr_dst != size)
	{
		pixels[ptr_dst] = std::min(255, (int)ppm[ptr_ppm]*16);
		pixels[ptr_dst+1] = std::min(255, (int)ppm[ptr_ppm+1]*16);
		pixels[ptr_dst+2] = std::min(255, (int)ppm[ptr_ppm+2]*16);
		//			memcpy(&pixels[ptr_dst], &ppm[ptr_ppm], 3);
		ptr_dst += 3;
		ptr_ppm += 3;

		pixels[ptr_dst] = std::min(255, (int)pgm[ptr_pgm]*16);
		//			memcpy(&pixels[ptr_dst], &pgm[ptr_pgm], 1);
		ptr_dst += 1;
		ptr_pgm += 1;
	}

	delete[] ppm;
	delete[] pgm;

	return pixels;
}

uint8_t* PPMAdapter::ReadPPM(const std::string& filename, int& width, int& height)
{
	std::locale::global(std::locale(""));
	std::ifstream fin(filename.c_str(), std::ios::binary);
	std::locale::global(std::locale("C"));
	assert(!fin.fail());

	uint8_t tag0, tag1;
	fin.read(reinterpret_cast<char*>(&tag0), sizeof(uint8_t));
	fin.read(reinterpret_cast<char*>(&tag1), sizeof(uint8_t));
	assert(tag0 == 80 && tag1 == 54);	// P6

	uint8_t skip;
	fin.read(reinterpret_cast<char*>(&skip), sizeof(uint8_t));

	// width
	width = 0;
	for (int i = 0; i < 4; ++i)
	{
		uint8_t n;
		fin.read(reinterpret_cast<char*>(&n), sizeof(uint8_t));
		width = width * 10 + (n - 48);
	}
	fin.read(reinterpret_cast<char*>(&skip), sizeof(uint8_t));
	// height
	height = 0;
	for (int i = 0; i < 4; ++i)
	{
		uint8_t n;
		fin.read(reinterpret_cast<char*>(&n), sizeof(uint8_t));
		height = height * 10 + (n - 48);
	}

	for (int i = 0; i < 4; ++i)
		fin.read(reinterpret_cast<char*>(&skip), sizeof(uint8_t));

	int size = width * height * 3;
	uint8_t* pixels = new uint8_t[size];
	fin.read(reinterpret_cast<char*>(pixels), size);

	fin.close();

	return pixels;
}

uint8_t* PPMAdapter::ReadPGM(const std::string& filename, int& width, int& height)
{
	std::locale::global(std::locale(""));
	std::ifstream fin(filename.c_str(), std::ios::binary);
	std::locale::global(std::locale("C"));
	assert(!fin.fail());

	uint8_t tag0, tag1;
	fin.read(reinterpret_cast<char*>(&tag0), sizeof(uint8_t));
	fin.read(reinterpret_cast<char*>(&tag1), sizeof(uint8_t));
	assert(tag0 == 80 && tag1 == 53);	// P5

	uint8_t skip;
	fin.read(reinterpret_cast<char*>(&skip), sizeof(uint8_t));

	// width
	width = 0;
	for (int i = 0; i < 4; ++i)
	{
		uint8_t n;
		fin.read(reinterpret_cast<char*>(&n), sizeof(uint8_t));
		width = width * 10 + (n - 48);
	}
	fin.read(reinterpret_cast<char*>(&skip), sizeof(uint8_t));
	// height
	height = 0;
	for (int i = 0; i < 4; ++i)
	{
		uint8_t n;
		fin.read(reinterpret_cast<char*>(&n), sizeof(uint8_t));
		height = height * 10 + (n - 48);
	}

	for (int i = 0; i < 4; ++i)
		fin.read(reinterpret_cast<char*>(&skip), sizeof(uint8_t));

	int size = width * height;
	uint8_t* pixels = new uint8_t[size];
	fin.read(reinterpret_cast<char*>(pixels), size);

	fin.close();

	return pixels;
}

}