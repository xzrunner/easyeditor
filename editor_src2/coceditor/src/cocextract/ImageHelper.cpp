#include "ImageHelper.h"

#include <SOIL/stb_image_write.h>

namespace cocextract
{
	ImageHelper::ImageHelper()
		: m_pixels(NULL)
	{
	}

	ImageHelper::~ImageHelper()
	{
		delete[] m_pixels;
	}

	const unsigned char* ImageHelper::clip(int xmin, int xmax, int ymin, int ymax)
	{
		int channels;
		int size = 0;
		switch(m_format)
		{
		case 0x1907:	// GL_RGB
			channels = 3;
			return NULL;
		case 0x1908:	// GL_RGBA
			channels = 4;
			break;
		default:
			return NULL;
		}

		if (xmin < 0 || xmin >= m_width ||
			xmax < 0 || xmax >= m_width ||
			ymin < 0 || ymin >= m_width ||
			ymax < 0 || ymax >= m_width ||
			xmin >= xmax || ymin >= ymax)
			return NULL;

		int w = xmax - xmin,
			h = ymax - ymin;
		unsigned char* pixels = new unsigned char[w * h * channels];
		int line_size = channels * w;
		for (int i = 0; i < h; ++i)
		{
			int from = (m_width * (ymin + i) + xmin) * channels,
				to = i * w * channels;
			memcpy(&pixels[to], &m_pixels[from], line_size);
		}
		return pixels;
	}

	void ImageHelper::writeToFile(const unsigned char* pixels, int width, int height,
		const std::string& filename)
	{
		assert(m_format == 0x1908);
		stbi_write_png(filename.c_str(), width, height, 4, pixels, 0);
	}

	void ImageHelper::loadPNG(const std::string& filename)
	{
		
	}

	void ImageHelper::loadPPM(const std::string& filename)
	{
		std::string filepath;
		int w0, h0, w1, h1;
		filepath = filename + ".ppm";
		unsigned char* ppm = loadPPM(filepath, w0, h0); 
		filepath = filename + ".pgm";
		unsigned char* pgm = loadPGM(filepath, w1, h1);
		assert(w0 == w1 && h0 == h1);

		m_width = w0;
		m_height = h0;
		m_format = 0x1908;	// GL_RGBA
		int size = m_width * m_height * 4;
		delete m_pixels;
		m_pixels = new unsigned char[size];
		
		int ptr_ppm = 0, ptr_pgm = 0, ptr_dst = 0;
		while (ptr_dst != size)
		{
 			m_pixels[ptr_dst] = std::min(255, (int)ppm[ptr_ppm]*16);
 			m_pixels[ptr_dst+1] = std::min(255, (int)ppm[ptr_ppm+1]*16);
 			m_pixels[ptr_dst+2] = std::min(255, (int)ppm[ptr_ppm+2]*16);
//			memcpy(&m_pixels[ptr_dst], &ppm[ptr_ppm], 3);
			ptr_dst += 3;
			ptr_ppm += 3;

			m_pixels[ptr_dst] = std::min(255, (int)pgm[ptr_pgm]*16);
//			memcpy(&m_pixels[ptr_dst], &pgm[ptr_pgm], 1);
			ptr_dst += 1;
			ptr_pgm += 1;
		}

		delete[] ppm;
		delete[] pgm;
	}

	void ImageHelper::load()
	{
// 		std::ifstream fin(m_filename.c_str(), std::ios::binary);
// 		assert(!fin.fail());
// 
// 		// get length of file:
// 		fin.seekg (0, fin.end);
// 		int length = fin.tellg();
// 		fin.seekg (0, fin.beg);
// 
// 		char* buffer = new char[length];
// 		fin.read (buffer,length);
// 		m_pixels = d2d::ImageLoader::loadData(m_filename.c_str(), m_width, m_height, m_format);
// 
// 		delete[] buffer;
	}

	unsigned char* ImageHelper::loadPPM(const std::string& filename, int& width, int& height)
	{
		std::ifstream fin(filename.c_str(), std::ios::binary);
		assert(!fin.fail());

		unsigned char tag0, tag1;
		fin.read(reinterpret_cast<char*>(&tag0), sizeof(unsigned char));
		fin.read(reinterpret_cast<char*>(&tag1), sizeof(unsigned char));
		assert(tag0 == 80 && tag1 == 54);	// P6

		unsigned char skip;
		fin.read(reinterpret_cast<char*>(&skip), sizeof(unsigned char));

		// width
		width = 0;
		for (int i = 0; i < 4; ++i)
		{
			unsigned char n;
			fin.read(reinterpret_cast<char*>(&n), sizeof(unsigned char));
			width = width * 10 + (n - 48);
		}
		fin.read(reinterpret_cast<char*>(&skip), sizeof(unsigned char));
		// height
		height = 0;
		for (int i = 0; i < 4; ++i)
		{
			unsigned char n;
			fin.read(reinterpret_cast<char*>(&n), sizeof(unsigned char));
			height = height * 10 + (n - 48);
		}

		for (int i = 0; i < 4; ++i)
			fin.read(reinterpret_cast<char*>(&skip), sizeof(unsigned char));

		int size = width * height * 3;
		unsigned char* pixels = new unsigned char[size];
		fin.read(reinterpret_cast<char*>(pixels), size);

		fin.close();

		return pixels;
	}

	unsigned char* ImageHelper::loadPGM(const std::string& filename, int& width, int& height)
	{
		std::ifstream fin(filename.c_str(), std::ios::binary);
		assert(!fin.fail());

		unsigned char tag0, tag1;
		fin.read(reinterpret_cast<char*>(&tag0), sizeof(unsigned char));
		fin.read(reinterpret_cast<char*>(&tag1), sizeof(unsigned char));
		assert(tag0 == 80 && tag1 == 53);	// P5

		unsigned char skip;
		fin.read(reinterpret_cast<char*>(&skip), sizeof(unsigned char));

		// width
		width = 0;
		for (int i = 0; i < 4; ++i)
		{
			unsigned char n;
			fin.read(reinterpret_cast<char*>(&n), sizeof(unsigned char));
			width = width * 10 + (n - 48);
		}
		fin.read(reinterpret_cast<char*>(&skip), sizeof(unsigned char));
		// height
		height = 0;
		for (int i = 0; i < 4; ++i)
		{
			unsigned char n;
			fin.read(reinterpret_cast<char*>(&n), sizeof(unsigned char));
			height = height * 10 + (n - 48);
		}

		for (int i = 0; i < 4; ++i)
			fin.read(reinterpret_cast<char*>(&skip), sizeof(unsigned char));

		int size = width * height;
		unsigned char* pixels = new unsigned char[size];
		fin.read(reinterpret_cast<char*>(pixels), size);

		fin.close();

		return pixels;
	}
}