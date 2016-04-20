#include "ImageLoader.h"
#include "StringHelper.h"
#include "config.h"
#include "SettingData.h"
#include "Exception.h"
#include "LibpngAdapter.h"
#include "LibjpegAdapter.h"
#include "PPMAdapter.h"
#include "ShaderMgr.h"
#include "EE_ShaderLab.h"

#include <dtex_pvr.h>

#include <gl/glew.h>

#include <assert.h>

namespace ee
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
	StringHelper::ToLower(type);
	if (type == "png")
	{
		data = LibpngAdapter::Read(filepath.c_str(), width, height, channels, format);
	}
	else if (type == "jpg")
	{
		data = LibjpegAdapter::Read(filepath.c_str(), width, height, channels, format);
	}
	else if (type == "ppm" || type == "pgm")
	{
		std::string filename = filepath.substr(0, filepath.find_last_of("."));
		channels = 4;
		data = PPMAdapter::Read(filename, width, height);
	}
	else if (type == "pvr")
	{
		uint32_t w, h;
		uint8_t* compressed = dtex_pvr_read_file(filepath.c_str(), &w, &h);
		uint8_t* uncompressed = dtex_pvr_decode(compressed, w, h);
		free(compressed);
		data = uncompressed;
		width = w;
		height = h;
		channels = 4;
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
	EE_TEXTURE_FORMAT ee_fmt;
	switch (format)
	{
	case GL_RGBA:
		ee_fmt = EE_TEXTURE_RGBA8;
		break;
	case GL_RGB:
		ee_fmt = EE_TEXTURE_RGB;
		break;
	case GL_LUMINANCE:
		ee_fmt = EE_TEXTURE_A8;
		break;
	default:
		throw ee::Exception("ImageLoader::PixelsToTexture Unknown format %d", format);
	}

	texture = ShaderLab::Instance()->CreateTexture(pixel, width, height, ee_fmt);
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
				pixels[pos + i] = static_cast<uint8_t>(pixels[pos + i] * alpha);
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