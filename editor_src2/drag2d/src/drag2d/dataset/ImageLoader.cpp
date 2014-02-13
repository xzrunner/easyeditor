#include "ImageLoader.h"

#include <libpng/png.h>
#include <assert.h>
#include <gl/glew.h>
#include <fstream>

namespace d2d
{
	int offset = 0;

	unsigned char* ImageLoader::loadTexture(void* data, int& width, int& height, unsigned int& texture, int& format)
	{
		unsigned char* pixel_data = loadPNG(data, width, height, format);
		assert(pixel_data);

		if (texture == 0)
		{
			glGenTextures(1,(GLuint*)&texture);
//			assert(texture);
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
			pixel_data);

		return pixel_data;
	}

	unsigned char* ImageLoader::loadData(const char* filepath, int& width, int& height, int& format)
	{
		std::ifstream fin(filepath, std::ios::binary);
		assert(!fin.fail());

		// get length of file:
		fin.seekg (0, fin.end);
		int length = fin.tellg();
		fin.seekg (0, fin.beg);

		char* buffer = new char[length];
		fin.read (buffer,length);
		fin.close();
		unsigned char* data = loadPNG(buffer, width, height, format);
		delete[] buffer;

		return data;
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
}