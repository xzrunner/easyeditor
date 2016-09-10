#include "PngLoader.h"
#include "Exception.h"

#include <libpng/png.h>
#include <assert.h>
#include <vector>
#include <fstream>

namespace gum
{

int PngLoader::Write(const uint8_t* pixels, int width, int height, const std::string& filename)
{
	unsigned bands_per_pixel = 4;

	png_byte color_type = 0;
	color_type = PNG_COLOR_TYPE_RGBA;

	int bit_depth = 8;

	FILE *out = fopen(filename.c_str(), "wb");
	if (!out) {
		return 1;
	}

	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	assert(png_ptr);
	png_infop info_ptr = png_create_info_struct(png_ptr);
	assert(info_ptr);
	png_init_io(png_ptr, out);
	png_set_IHDR(png_ptr, info_ptr, width, height,
		bit_depth, color_type, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	png_write_info(png_ptr, info_ptr);

	std::vector<uint8_t*> row_pointers(height);
	for (int y = 0; y < height; y++) {
		// reverse
		row_pointers[height - 1 - y] = (uint8_t*)pixels + y * (width * bands_per_pixel * bit_depth / 8);
	}

	png_write_image(png_ptr, &row_pointers[0]);
	png_write_end(png_ptr, NULL);
	fclose(out);

	return 0;
}


int offset = 0;
void read_memory_cb(png_structp png, png_bytep data, png_size_t size)
{
	char* raw = (char*)png_get_io_ptr(png);
	memcpy(data, raw + offset, size);
	offset += size;
}

uint8_t* PngLoader::Read(const std::string& filename, int& width, int& height, TEXTURE_FORMAT& format)
{
	std::locale::global(std::locale(""));
	std::ifstream fin(filename.c_str(), std::ios::binary);
	std::locale::global(std::locale("C"));
	if (fin.fail()) {
		return NULL;
	}

	// get length of file:
	fin.seekg(0, fin.end);
	int length = fin.tellg();
	fin.seekg(0, fin.beg);

	char* data = new char[length];
	fin.read(data,length);
	fin.close();

	offset = 0;

	png_byte header[8];
	png_structp png_ptr = NULL; png_infop info_ptr = NULL;
	png_byte* buffer = NULL; png_bytep* row_ptr = NULL;
	png_int_32 row_size; 
	bool transparency;

	do
	{
// 		if (m_resource.read(header, sizeof(header)) == 0)
// 			break;
		memcpy(header, (char*)data + offset, sizeof(header));
		offset += sizeof(header);
		if (png_sig_cmp(header, 0, 8) != 0) break;

		png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (!png_ptr) break;
		info_ptr = png_create_info_struct(png_ptr);
		if (!info_ptr) break;
		png_set_read_fn(png_ptr, data, read_memory_cb);
		if (setjmp(png_jmpbuf(png_ptr))) break;

		png_set_sig_bytes(png_ptr, 8);
		png_read_info(png_ptr, info_ptr);

		png_int_32 _depth, _color_type;
		png_uint_32 _width, _height;
		png_get_IHDR(png_ptr, info_ptr, &_width, &_height, &_depth, &_color_type, NULL, NULL, NULL);
		width = _width; height = _height;

		// Creates a full alpha channel if transparency is encoded as
		// an array of palette entries or a single transparent color.
		transparency = false;
		if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) 
		{
			png_set_tRNS_to_alpha(png_ptr);
			transparency = true;

			// for read pngquant's 256 color image
			// 				break;
		}
		// Expands PNG with less than 8bits per channel to 8bits.
		if (_depth < 8) 
		{
			png_set_packing (png_ptr);
		} 
		// Shrinks PNG with 16bits per color channel down to 8bits.
		else if (_depth == 16) 
		{
			png_set_strip_16(png_ptr);
		}
		// Indicates that image needs conversion to RGBA if needed.
		switch (_color_type) 
		{
		case PNG_COLOR_TYPE_PALETTE:
			png_set_palette_to_rgb(png_ptr);
			//format = lTransparency ? GL_RGBA : GL_RGB;
			format = transparency ? TEXTURE_RGBA8 : TEXTURE_RGB;
			break;
		case PNG_COLOR_TYPE_RGB:
			//format = lTransparency ? GL_RGBA : GL_RGB;
			format = transparency ? TEXTURE_RGBA8 : TEXTURE_RGB;
			break;
		case PNG_COLOR_TYPE_RGBA:
			//format = GL_RGBA;
			format = TEXTURE_RGBA8;
			break;
		case PNG_COLOR_TYPE_GRAY:
			png_set_expand_gray_1_2_4_to_8(png_ptr);
			//format = lTransparency ? GL_LUMINANCE_ALPHA : GL_LUMINANCE;
			format = TEXTURE_A8;
			break;
		case PNG_COLOR_TYPE_GA:
			png_set_expand_gray_1_2_4_to_8(png_ptr);
			//format = GL_LUMINANCE_ALPHA;
			format = TEXTURE_A8;
			break;
		}
		png_read_update_info(png_ptr, info_ptr);

		row_size = png_get_rowbytes(png_ptr, info_ptr);
		if (row_size <= 0) break;
		buffer = new png_byte[row_size * _height];
		if (!buffer) break;
		row_ptr = new png_bytep[_height];
		if (!row_ptr) break;
		for (unsigned int i = 0; i < _height; ++i) 
		{
			row_ptr[_height - (i + 1)] = buffer + i * row_size;
		}
		png_read_image(png_ptr, row_ptr);

		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		delete[] row_ptr;

		delete[] data;

		return buffer;
	} while (0);

	// error
	//ERROR:
	//		Log::error("Error while reading PNG file");
	delete[] row_ptr; delete[] buffer;
	delete[] data;
	if (png_ptr != NULL) 
	{
		png_infop* info_pp = info_ptr != NULL ? &info_ptr : NULL;
		png_destroy_read_struct(&png_ptr, info_pp, NULL);
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

void PngLoader::ReadHeader(const std::string& filename, int& width, int& height)
{
	std::locale::global(std::locale(""));
	std::ifstream fin(filename.c_str(), std::ios::binary);
	std::locale::global(std::locale("C"));
	if (fin.fail()) {
		throw Exception("Can't open %s\n", filename.c_str());
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