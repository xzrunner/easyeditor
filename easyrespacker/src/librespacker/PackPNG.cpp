#include "PackPNG.h"
#include "Lzma.h"
#include "typedef.h"

#include <drag2d.h>

namespace librespacker
{

PackPNG::PackPNG(bool png8)
	: m_buffer(NULL)
	, m_buf_sz(0)
{
	m_type = png8 ? TT_PNG8 : TT_PNG4;
}

PackPNG::~PackPNG()
{
	Clear();
}

void PackPNG::Load(const std::string& filepath)
{
	Clear();

	int w, h, c, f;
	uint8_t* buf = d2d::ImageLoader::FileToPixels(filepath, w, h, c, f);

	m_width = w;
	m_height = h;
	m_buffer = buf;
	m_buf_sz = w * h * c;

	if (m_type == TT_PNG4) {
		GenPng4();
	}
}

// void PackPNG::Load(const std::string& filepath)
// {
// 	png_image image;
// 
// 	memset(&image, 0, (sizeof image));
// 	image.version = PNG_IMAGE_VERSION;
// 
// 	if (png_image_begin_read_from_file(&image, filepath.c_str()) != 0)
// 	{
// 		png_bytep buffer;
// 
//          /* Set the format in which to read the PNG file; this code chooses a
//           * simple sRGB format with a non-associated alpha channel, adequate to
//           * store most images.
//           */
// 		image.format = PNG_FORMAT_RGBA;
// 
// 		int sz = PNG_IMAGE_SIZE(image);
// 		buffer = (png_bytep)malloc(sz);
// 		if (buffer != NULL && 
// 			png_image_finish_read(&image, NULL, buffer, 0, NULL) != 0)
// 		{
// 			m_width = image.width;
// 			m_height = image.height;
// 
// 			m_buffer = new uint8_t[sz];
// 			memcpy(m_buffer, buffer, sz);
// 			m_buf_sz = sz;
// 			if (m_type == TT_PNG4) {
// 				GenPng4();
// 			}
// 		}
// 	}
// }

void PackPNG::Store(std::ofstream& fout, float scale) const
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
		int sz = 0;
		sz += sizeof(int8_t);	// type
		sz += sizeof(int16_t);	// width
		sz += sizeof(int16_t);	// height
		sz += m_buf_sz;			// buf
		sz = -sz;
		fout.write(reinterpret_cast<const char*>(&sz), sizeof(int32_t));

		fout.write(reinterpret_cast<const char*>(&m_type), sizeof(int8_t));
		fout.write(reinterpret_cast<const char*>(&m_width), sizeof(int16_t));
		fout.write(reinterpret_cast<const char*>(&m_height), sizeof(int16_t));
		fout.write(reinterpret_cast<const char*>(m_buffer), m_buf_sz);
	}
}

void PackPNG::Clear()
{
	delete[] m_buffer, m_buffer = NULL;
	m_buf_sz = 0;
}

static inline int
_round(int c) {
	c = (c + 7 > 0xff) ? (0xff) : (c + 7); 
	return c >> 4;
}

// todo
void PackPNG::GenPng4()
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