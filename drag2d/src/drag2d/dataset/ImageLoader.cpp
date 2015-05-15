#include <string>

#include "ImageLoader.h"

#include <SOIL/SOIL.h>
#include <libpng/png.h>
#include <assert.h>
#include <fstream>

#define NOMINMAX
#include <gl/glew.h>

#include "common/tools.h"
#include "render/ShaderMgr.h"

//#define USE_SOIL

namespace d2d
{

uint8_t* ImageLoader::loadTexture(const std::string& filepath, int& width, int& height, unsigned int& texture, int& channels)
{
	int format;
	uint8_t* pixel_data = loadData(filepath, width, height, channels, format);
	assert(pixel_data);
	loadTexture(texture, pixel_data, width, height, channels, format);
	return pixel_data;
}

uint8_t* ImageLoader::loadData(const std::string& filepath, int& width, int& height, int& channels, int& format)
{
	uint8_t* data = NULL;

	format = 0;

	std::string type = filepath.substr(filepath.find_last_of(".") + 1);
	StringTools::toLower(type);
	if (type == "png")
	{
		// todo: 用libpng读出来的数据再导出时有的会乱掉
#ifdef USE_SOIL
		data = loadImageBySOIL(filepath, width, height, channels);
#else
		data = loadImageByLibpng(filepath, width, height, channels, format);
#endif // USE_SOIL
	}
	else if (type == "ppm" || type == "pgm")
	{
		std::string filename = filepath.substr(0, filepath.find_last_of("."));
		channels = 4;
		data = loadPNM(filename, width, height);
	}
	else
	{
		data = loadImageBySOIL(filepath, width, height, channels);
	}

	if (channels == 4) {
		FormatPixelsAlpha(data, width, height, 255);
	}

	if (format == 0)
	{
		switch (channels)
		{
		case 4:
			format = GL_RGBA;
			break;
		case 3:
			format = GL_RGB;
			break;
		default:
			format = GL_LUMINANCE;
		}
	}

	return data;
}

void ImageLoader::FormatPixelsAlpha(uint8_t* pixels, int width, int height, int val)
{
	int ptr = 0;
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			uint8_t r = pixels[ptr],
					g = pixels[ptr+1],
					b = pixels[ptr+2],
					a = pixels[ptr+3];
			if (a == 0) {
				r = g = b = val;
			}
			pixels[ptr++] = r;
			pixels[ptr++] = g;
			pixels[ptr++] = b;
			pixels[ptr++] = a;
		}
	}
}

void ImageLoader::loadTexture(unsigned int& texture, const uint8_t* pixel, int width, int height, int channels, int format)
{
	//// todo: 当数据用SOIL导入时，再这个方法导入的纹理时是黑的
#ifdef USE_SOIL
	texture = SOIL_create_OGL_texture
		(
		pixel,
		width, height, channels,
		texture,
		SOIL_FLAG_INVERT_Y 
		);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#else
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

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

 	if (format == GL_RGB) {
 		format = GL_RGBA;
 		
 		uint8_t* fixed = new uint8_t[width * height * 4];
 		int size = width * height;
 		int ptr_src = 0, ptr_dst = 0;
 		for (int i = 0; i < size; ++i) {
 			for (int i = 0; i < 3; ++i) {
 				fixed[ptr_dst++] = pixel[ptr_src++];
 			}
 			fixed[ptr_dst++] = 255;
 		}
 
 		glTexImage2D(GL_TEXTURE_2D,
 			0,
 			format,
 			(GLsizei)width,
 			(GLsizei)height,
 			0,
 			format,
 			GL_UNSIGNED_BYTE,
 			fixed);
 
 		delete[] fixed;
 	} else {
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

	glBindTexture(GL_TEXTURE_2D, ShaderMgr::Instance()->GetTexID());
#endif // USE_SOIL
}

int offset = 0;
void callback_read(png_structp png, png_bytep data, png_size_t size)
{
	char* raw = (char*) png_get_io_ptr(png);
	memcpy(data, raw + offset, size);
	offset += size;
}

uint8_t* ImageLoader::loadImageByLibpng(const std::string& filename, int& width, int& height, int& channels, int& format)
{
	std::locale::global(std::locale(""));
	std::ifstream fin(filename.c_str(), std::ios::binary);
	std::locale::global(std::locale("C"));
	assert(!fin.fail());

	// get length of file:
	fin.seekg (0, fin.end);
	int length = fin.tellg();
	fin.seekg (0, fin.beg);

	char* data = new char[length];
	fin.read (data,length);
	fin.close();
	
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
			channels = lTransparency ? 4 : 3;
			break;
		case PNG_COLOR_TYPE_RGB:
			format = lTransparency ? GL_RGBA : GL_RGB;
			channels = lTransparency ? 4 : 3;
			break;
		case PNG_COLOR_TYPE_RGBA:
			format = GL_RGBA;
			channels = 4;
			break;
		case PNG_COLOR_TYPE_GRAY:
			png_set_expand_gray_1_2_4_to_8(lPngPtr);
			format = lTransparency ? GL_LUMINANCE_ALPHA : GL_LUMINANCE;
			channels = 1;
			break;
		case PNG_COLOR_TYPE_GA:
			png_set_expand_gray_1_2_4_to_8(lPngPtr);
			format = GL_LUMINANCE_ALPHA;
			channels = 1;
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

		delete[] data;

		return lImageBuffer;
	} while (0);

	// error
	//ERROR:
	//		Log::error("Error while reading PNG file");
	delete[] lRowPtrs; delete[] lImageBuffer;
	delete[] data;
	if (lPngPtr != NULL) 
	{
		png_infop* lInfoPtrP = lInfoPtr != NULL ? &lInfoPtr : NULL;
		png_destroy_read_struct(&lPngPtr, lInfoPtrP, NULL);
	}
	return NULL;
}

uint8_t* ImageLoader::loadImageBySOIL(const std::string& filename, int& width, int& height, int& channels)
{
	uint8_t* pixels = SOIL_load_image(filename.c_str(), &width, &height, &channels, 0);

	// invert y
	int size = width*height*channels;
	uint8_t* inverted = new uint8_t[size];
	int line_size = width * channels;
	int ptr = size - line_size;
	while (ptr > 0)
	{
		memcpy(&inverted[size - ptr], &pixels[ptr], line_size);
		ptr -= line_size;
	}
	
	delete[] pixels;
	return inverted;
}

uint8_t* ImageLoader::loadPNM(const std::string& filename, int& width, int& height)
{
	std::string filepath;
	int w0, h0, w1, h1;
	filepath = filename + ".ppm";
	uint8_t* ppm = loadPPM(filepath, w0, h0); 
	filepath = filename + ".pgm";
	uint8_t* pgm = loadPGM(filepath, w1, h1);
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

uint8_t* ImageLoader::loadPPM(const std::string& filename, int& width, int& height)
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

uint8_t* ImageLoader::loadPGM(const std::string& filename, int& width, int& height)
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