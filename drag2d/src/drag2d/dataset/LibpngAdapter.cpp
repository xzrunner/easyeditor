#include "LibpngAdapter.h"
#include "common/Exception.h"

#include <libpng/png.h>
#include <assert.h>
#include <vector>
#include <fstream>
#include <gl/glew.h>

namespace d2d
{

int LibpngAdapter::Write(const uint8_t* pixels, int width, int height, 
						 const std::string& filename, bool reverse)
{
	unsigned bands_per_pixel = 4;

	png_byte color_type = 0;
	color_type = PNG_COLOR_TYPE_RGBA;

	int bit_depth = 8;

	FILE *out = fopen(filename.c_str(), "wb");
	if (!out) {
		return 1;
	}

	png_structp p_str = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	assert(p_str);
	png_infop p_info = png_create_info_struct(p_str);
	assert(p_info);
	png_init_io(p_str, out);
	png_set_IHDR(p_str, p_info, width, height,
		bit_depth, color_type, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	png_write_info(p_str, p_info);

	std::vector<uint8_t*> row_pointers(height);
	if (reverse) {
		for (unsigned y = 0; y < height; y++) {
			row_pointers[height - 1 - y] = (uint8_t*)pixels + y * (width * bands_per_pixel * bit_depth / 8);
		}
	} else {
		for (unsigned y = 0; y < height; y++) {
			row_pointers[height] = (uint8_t*)pixels + y * (width * bands_per_pixel * bit_depth / 8);
		}
	}

	png_write_image(p_str, &row_pointers[0]);
	png_write_end(p_str, NULL);
	fclose(out);

	return 0;
}


int offset = 0;
void read_memory_cb(png_structp png, png_bytep data, png_size_t size)
{
	char* raw = (char*) png_get_io_ptr(png);
	memcpy(data, raw + offset, size);
	offset += size;
}

uint8_t* LibpngAdapter::Read(const std::string& filename, int& width, int& height, 
							 int& channels, int& format)
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
		png_set_read_fn(lPngPtr, data, read_memory_cb);
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

void read_file_cb(png_structp png_ptr, png_bytep out, png_size_t count)
{
	png_voidp io_ptr = png_get_io_ptr( png_ptr );

	if( io_ptr == 0 )
	{
		return;
	}

	std::ifstream &ifs = *(std::ifstream*)io_ptr;

	ifs.read( (char*)out, count );
}

void LibpngAdapter::ReadHeader(const std::string& filename, int& width, int& height)
{
	std::locale::global(std::locale(""));
	std::ifstream fin(filename.c_str(), std::ios::binary);
	std::locale::global(std::locale("C"));
	if (fin.fail()) {
		throw d2d::Exception("Can't open %s\n", filename.c_str());
	}

	png_byte header[8];
	fin.read(reinterpret_cast<char*>(&header[0]), sizeof(header));
	if (png_sig_cmp(header, 0, 8) != 0) {
		return;
	}

	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr) {
		return;
	}
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		return;
	}
	png_set_read_fn(png_ptr, (void*)&fin, read_file_cb);
	if (setjmp(png_jmpbuf(png_ptr))) {
		return;
	}

	png_set_sig_bytes(png_ptr, 8);

	png_read_info(png_ptr, info_ptr);

	png_int_32 _depth, _color_type;
	png_uint_32 _width, _height;
	png_get_IHDR(png_ptr, info_ptr, &_width, &_height, &_depth, &_color_type, NULL, NULL, NULL);

	width = _width; height = _height;

	if (png_ptr != NULL) 
	{
		png_infop* info_ptr_ptr = info_ptr != NULL ? &info_ptr : NULL;
		png_destroy_read_struct(&png_ptr, info_ptr_ptr, NULL);
	}
}

}