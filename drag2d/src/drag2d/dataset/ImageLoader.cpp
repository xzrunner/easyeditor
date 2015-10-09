#include "ImageLoader.h"
#include "common/StringTools.h"
#include "common/config.h"
#include "common/SettingData.h"
#include "common/Exception.h"
#include "render/ShaderMgr.h"

#include "LibpngAdapter.h"
#include "PPMAdapter.h"

#include <assert.h>
#include <gl/glew.h>

namespace d2d
{

uint8_t* ImageLoader::FileToTexture(const std::string& filepath, int& width, int& height, unsigned int& texture, int& channels)
{
	int format;
	uint8_t* pixel_data = FileToPixels(filepath, width, height, channels, format);
	assert(pixel_data);
	PixelsToTexture(texture, pixel_data, width, height, channels, format);
	return pixel_data;
}

uint8_t* ImageLoader::FileToPixels(const std::string& filepath, int& width, int& height, int& channels, int& format)
{
	uint8_t* data = NULL;

	format = 0;

	std::string type = filepath.substr(filepath.find_last_of(".") + 1);
	StringTools::ToLower(type);
	if (type == "png")
	{
		data = LibpngAdapter::Read(filepath, width, height, channels, format);
	}
	else if (type == "ppm" || type == "pgm")
	{
		std::string filename = filepath.substr(0, filepath.find_last_of("."));
		channels = 4;
		data = PPMAdapter::Read(filename, width, height);
	}
	else
	{
		throw Exception("unknown type %s", type.c_str());
	}

	if (channels == 4) {
		RemoveGhostPixel(data, width, height);
		FormatPixelsAlpha(data, width, height, 0);
		if (Config::Instance()->GetSettings().pre_multi_alpha) {
			PreMuiltiAlpha(data, width, height);
		}
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

void ImageLoader::PixelsToTexture(unsigned int& texture, const uint8_t* pixel, int width, int height, int channels, int format)
{
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

void ImageLoader::PreMuiltiAlpha(uint8_t* pixels, int width, int height)
{
	int pos = 0;
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			float alpha = pixels[pos + 3] / 255.0f;
			for (int i = 0; i < 3; ++i) {
				pixels[pos + i] = pixels[pos + i] * alpha;
			}
			pos += 4;
		}
	}
}

void ImageLoader::RemoveGhostPixel(uint8_t* pixels, int width, int height)
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

// 			if (r <= 3 && g <= 3 && b <= 3 && a <= 3) {
// 				r = g = b = a = 0;
// 			}
			if (a <= 3) {
				a = 0;
			}

			pixels[ptr++] = r;
			pixels[ptr++] = g;
			pixels[ptr++] = b;
			pixels[ptr++] = a;
		}
	}
}

}