#include "PngLoader.h"
#include "image_type.h"

#include <png.h>

namespace epbin
{

PngLoader::PngLoader(bool png8)
{
	m_type = png8 ? TYPE_TEXTURE8 : TYPE_TEXTURE4;
}

void PngLoader::Load(const std::string& filepath)
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
			memcpy(m_buffer, buffer, sz);
			m_buf_sz = sz;
			if (m_type == TYPE_TEXTURE4) {
				GenPng4();
			}
		}
	}
}

static inline int
_round(int c) {
	c = (c + 7 > 0xff) ? (0xff) : (c + 7); 
	return c >> 4;
}

void PngLoader::GenPng4()
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