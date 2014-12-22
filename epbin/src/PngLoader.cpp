#include "PNGLoader.h"
#include "Lzma.h"
#include "image_type.h"

#include <png.h>

namespace epbin
{

PNGLoader::PNGLoader(bool png8)
	: m_buffer(NULL)
	, m_buf_sz(0)
{
	m_type = png8 ? TYPE_TEXTURE8 : TYPE_TEXTURE4;
}

PNGLoader::~PNGLoader()
{
	delete[] m_buffer;
}

void PNGLoader::Load(const std::string& filepath)
{
	png_image image;

	memset(&image, 0, (sizeof image));
	image.version = PNG_IMAGE_VERSION;

	if (png_image_begin_read_from_file(&image, filepath.c_str()) != 0)
	{
		png_bytep buffer;

         /* Set the format in which to read the PNG file; this code chooses a
          * simple sRGB format with a non-associated alpha channel, adequate to
          * store most images.
          */
		image.format = PNG_FORMAT_RGBA;

		int sz = PNG_IMAGE_SIZE(image);
		buffer = (png_bytep)malloc(sz);
		if (buffer != NULL && 
			png_image_finish_read(&image, NULL, buffer, 0, NULL) != 0)
		{
			m_width = image.width;
			m_height = image.height;

			m_buffer = new uint8_t[sz];
			memcpy(m_buffer, buffer, sz);
			m_buf_sz = sz;
			if (m_type == TYPE_TEXTURE4) {
				GenPng4();
			}
		}
	}
}

void PNGLoader::Store(std::ofstream& fout) const
{
	if (m_compress)
	{
		size_t sz = sizeof(int8_t) + sizeof(int16_t) * 2 + m_buf_sz;
		uint8_t* buf = new uint8_t[sz];
		uint8_t* ptr = buf;

		memcpy(ptr, &m_type, sizeof(int8_t));
		ptr += sizeof(int8_t);
		memcpy(ptr, &m_width, sizeof(int16_t));
		ptr += sizeof(int16_t);
		memcpy(ptr, &m_height, sizeof(int16_t));
		ptr += sizeof(int16_t);
		memcpy(ptr, m_buffer, m_buf_sz);
		ptr += m_buf_sz;

		uint8_t* dst = NULL;
		size_t dst_sz;
		Lzma::Compress(&dst, &dst_sz, buf, sz);
		delete[] buf;

		fout.write(reinterpret_cast<const char*>(&dst_sz), sizeof(uint32_t));
		fout.write(reinterpret_cast<const char*>(dst), dst_sz);
		delete[] dst;
	}
	else
	{
		fout.write(reinterpret_cast<const char*>(&m_type), sizeof(int8_t));
		fout.write(reinterpret_cast<const char*>(&m_width), sizeof(int16_t));
		fout.write(reinterpret_cast<const char*>(&m_height), sizeof(int16_t));
		fout.write(reinterpret_cast<const char*>(m_buffer), m_buf_sz);
	}
}

static inline int
_round(int c) {
	c = (c + 7 > 0xff) ? (0xff) : (c + 7); 
	return c >> 4;
}

void PNGLoader::GenPng4()
{
	for (int i = 0; i < m_buf_sz; i += 4)
	{
		int r = _round(m_buffer[i]);  
		int g = _round(m_buffer[i+1]);
		int b = _round(m_buffer[i+2]);
		int a = _round(m_buffer[i+3]);
	}
}

}