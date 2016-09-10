#include "ImageLoader.h"
#include "StringHelper.h"
#include "config.h"
#include "SettingData.h"
#include "Exception.h"
#include "LibpngAdapter.h"
#include "LibjpegAdapter.h"
#include "PPMAdapter.h"
#include "EE_ShaderLab.h"

#include <gum/ImageLoader.h>

#include <dtex_pvr.h>
#include <dtex_etc2.h>

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
	else if (type == "pkm")
	{
		uint32_t w, h;
		int type;
		uint8_t* compressed = dtex_etc2_read_file(filepath.c_str(), &w, &h, &type);
		uint8_t* uncompressed = dtex_etc2_decode(compressed, w, h, type);
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
		gum::ImageLoader::RemoveGhostPixel(data, width, height);
		gum::ImageLoader::FormatPixelsAlpha(data, width, height, 0);
		if (Config::Instance()->GetSettings().pre_multi_alpha) {
			gum::ImageLoader::PreMuiltiAlpha(data, width, height);
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

}