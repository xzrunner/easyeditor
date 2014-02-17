#include <string>

#include "ImageLoader.h"

#include <libpng/png.h>
#include <assert.h>
#include <gl/glew.h>
#include <fstream>

#include "common/tools.h"

namespace d2d
{
	int offset = 0;

	unsigned char* ImageLoader::loadTexture(void* data, int& width, int& height, unsigned int& texture, int& format)
	{
		unsigned char* pixel_data = loadPNG(data, width, height, format);
		assert(pixel_data);
		loadTexture(texture, pixel_data, format, width, height);
		return pixel_data;
	}

	unsigned char* ImageLoader::loadTexture(const std::string& filepath, int& width, int& height, unsigned int& texture, int& format)
	{
		unsigned char* pixel_data = loadData(filepath, width, height, format);
		assert(pixel_data);
		loadTexture(texture, pixel_data, format, width, height);
		return pixel_data;
	}

	unsigned char* ImageLoader::loadData(const std::string& filepath, int& width, int& height, int& format)
	{
		unsigned char* data = NULL;

		std::string type = filepath.substr(filepath.find_last_of(".") + 1);
		StringTools::toLower(type);
		if (type == "png")
		{
			std::ifstream fin(filepath.c_str(), std::ios::binary);
			assert(!fin.fail());

			// get length of file:
			fin.seekg (0, fin.end);
			int length = fin.tellg();
			fin.seekg (0, fin.beg);

			char* buffer = new char[length];
			fin.read (buffer,length);
			fin.close();
			data = loadPNG(buffer, width, height, format);
			delete[] buffer;
		}
		else if (type == "ppm" || type == "pgm")
		{
			std::string filename = filepath.substr(0, filepath.find_last_of("."));
			data = loadPPM(filename, width, height, format);
		}

		return data;
	}

	void ImageLoader::loadTexture(unsigned int& texture, unsigned char* pixel, int format, int width, int height)
	{
		if (texture == 0)
		{
			glGenTextures(1,(GLuint*)&texture);
			//assert(texture);
		}

		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D,
			0,
			format,
			(GLsizei)width,
			(GLsizei)height,
			0,
			format,
			GL_UNSIGNED_BYTE,
			pixel);
	}

	void callback_read(png_structp png, png_bytep data, png_size_t size)
	{
		char* raw = (char*) png_get_io_ptr(png);
		memcpy(data, raw + offset, size);
		offset += size;
	}

	unsigned char* ImageLoader::loadPNG(void* data, int& width, int& height, int& format)
	{
		offset = 0;

		png_byte lHeader[8];
		png_structp lPngPtr = NULL; png_infop lInfoPtr = NULL;
		png_byte* lImageBuffer = NULL; png_bytep* lRowPtrs = NULL;
		png_int_32 lRowSize; bool lTransparency;

		do
		{
			// 		if (m_resource.read(lHeader, sizeof(lHeader)) == 0)
			// 			break;
			memcpy(lHeader, (char*)data + offset, sizeof(lHeader));
			offset += sizeof(lHeader);
			if (png_sig_cmp(lHeader, 0, 8) != 0) break;

			lPngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
			if (!lPngPtr) break;
			lInfoPtr = png_create_info_struct(lPngPtr);
			if (!lInfoPtr) break;
			png_set_read_fn(lPngPtr, data, callback_read);
			if (setjmp(png_jmpbuf(lPngPtr))) break;

			png_set_sig_bytes(lPngPtr, 8);
			png_read_info(lPngPtr, lInfoPtr);
			png_int_32 lDepth, lColorType;
			png_uint_32 lWidth, lHeight;
			png_get_IHDR(lPngPtr, lInfoPtr, &lWidth, &lHeight, &lDepth, &lColorType, NULL, NULL, NULL);
			width = lWidth; height = lHeight;

			// Creates a full alpha channel if transparency is encoded as
			// an array of palette entries or a single transparent color.
			lTransparency = false;
			if (png_get_valid(lPngPtr, lInfoPtr, PNG_INFO_tRNS)) 
			{
				png_set_tRNS_to_alpha(lPngPtr);
				lTransparency = true;

				// for read pngquant's 256 color image
				// 				break;
			}
			// Expands PNG with less than 8bits per channel to 8bits.
			if (lDepth < 8) 
			{
				png_set_packing (lPngPtr);
			} 
			// Shrinks PNG with 16bits per color channel down to 8bits.
			else if (lDepth == 16) 
			{
				png_set_strip_16(lPngPtr);
			}
			// Indicates that image needs conversion to RGBA if needed.
			switch (lColorType) 
			{
			case PNG_COLOR_TYPE_PALETTE:
				png_set_palette_to_rgb(lPngPtr);
				format = lTransparency ? GL_RGBA : GL_RGB;
				break;
			case PNG_COLOR_TYPE_RGB:
				format = lTransparency ? GL_RGBA : GL_RGB;
				break;
			case PNG_COLOR_TYPE_RGBA:
				format = GL_RGBA;
				break;
			case PNG_COLOR_TYPE_GRAY:
				png_set_expand_gray_1_2_4_to_8(lPngPtr);
				format = lTransparency ? GL_LUMINANCE_ALPHA : GL_LUMINANCE;
				break;
			case PNG_COLOR_TYPE_GA:
				png_set_expand_gray_1_2_4_to_8(lPngPtr);
				format = GL_LUMINANCE_ALPHA;
				break;
			}
			png_read_update_info(lPngPtr, lInfoPtr);

			lRowSize = png_get_rowbytes(lPngPtr, lInfoPtr);
			if (lRowSize <= 0) break;
			lImageBuffer = new png_byte[lRowSize * lHeight];
			if (!lImageBuffer) break;
			lRowPtrs = new png_bytep[lHeight];
			if (!lRowPtrs) break;
			for (unsigned int i = 0; i < lHeight; ++i) 
			{
				lRowPtrs[lHeight - (i + 1)] = lImageBuffer + i * lRowSize;
			}
			png_read_image(lPngPtr, lRowPtrs);

			png_destroy_read_struct(&lPngPtr, &lInfoPtr, NULL);
			delete[] lRowPtrs;

			return lImageBuffer;
		} while (0);

		// error
		//ERROR:
		//		Log::error("Error while reading PNG file");
		delete[] lRowPtrs; delete[] lImageBuffer;
		if (lPngPtr != NULL) 
		{
			png_infop* lInfoPtrP = lInfoPtr != NULL ? &lInfoPtr : NULL;
			png_destroy_read_struct(&lPngPtr, lInfoPtrP, NULL);
		}
		return NULL;
	}

	unsigned char* ImageLoader::loadPPM(const std::string& filename, int& width, int& height, int& format)
	{
		std::string filepath;
		int w0, h0, w1, h1;
		filepath = filename + ".ppm";
		unsigned char* ppm = loadPPM(filepath, w0, h0); 
		filepath = filename + ".pgm";
		unsigned char* pgm = loadPGM(filepath, w1, h1);
		assert(w0 == w1 && h0 == h1);

		width = w0;
		height = h0;
		format = 0x1908;	// GL_RGBA
		int size = width * height * 4;
		unsigned char* pixels = new unsigned char[size];

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

	unsigned char* ImageLoader::loadPPM(const std::string& filename, int& width, int& height)
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

	unsigned char* ImageLoader::loadPGM(const std::string& filename, int& width, int& height)
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