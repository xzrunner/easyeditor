#include "ExtrudeImg.h"

#include <fstream>
#include <assert.h>
#include <SOIL/SOIL.h>
#include <SOIL/stb_image_write.h>

namespace coceditor {

ExtrudeImg::ExtrudeImg(const std::string& filename)
	: _filename(filename)
	, _pixels(NULL)
	, _trimed(NULL)
	, _extruded(NULL)
{
	Load();
}

ExtrudeImg::~ExtrudeImg()
{
	delete[] _pixels;
	delete[] _trimed;
	delete[] _extruded;
}

void ExtrudeImg::Trigger(int extrude)
{
	Trim();
//	Extrude(extrude);
}

std::string ExtrudeImg::GetFileName() const
{
	return ee::FilenameTools::getFilenameWithExtension(_filename);
}

void ExtrudeImg::Load()
{
	std::ifstream fin(_filename.c_str(), std::ios::binary);
	assert(!fin.fail());

	// get length of file:
	fin.seekg (0, fin.end);
	int length = fin.tellg();
	fin.seekg (0, fin.beg);

	char* buffer = new char[length];
	fin.read (buffer,length);
	_pixels = ee::ImageLoader::loadData(_filename.c_str(), _width, _height, _format);

	delete[] buffer;
}

void ExtrudeImg::Trim()
{
	int channels;
	int size = 0;
	switch(_format)
	{
	case 0x1907:	// GL_RGB
		channels = 3;
		size = _width * _height * channels;
		return;
	case 0x1908:	// GL_RGBA
		channels = 4;
		size = _width * _height * channels;
		break;
	default:
		return;
	}

	if (!size) return;
	
	// down
	_trimed_region.yMin = 0;
	for (size_t i = 0; i < _height; ++i)
	{
		size_t j = 0;
		for ( ; j < _width; ++j)
			if (!IsTransparent(_pixels, j, i, channels))
				break;
		if (j == _width) ++_trimed_region.yMin;
		else break;
	}
	// up
	_trimed_region.yMax = _height;
	for (int i = _height - 1; i >= 0; --i)
	{
		size_t j = 0;
		for ( ; j < _width; ++j)
			if (!IsTransparent(_pixels, j, i, channels))
				break;
		if (j == _width) --_trimed_region.yMax;
		else break;
	}
	// left
	_trimed_region.xMin = 0;
	for (size_t i = 0; i < _width; ++i)
	{
		size_t j = 0;
		for ( ; j < _height; ++j)
			if (!IsTransparent(_pixels, i, j, channels))
				break;
		if (j == _height) ++_trimed_region.xMin;
		else break;
	}
	// right
	_trimed_region.xMax = _width;
	for (int i = _width - 1; i >= 0; --i)
	{
		size_t j = 0;
		for ( ; j < _height; ++j)
			if (!IsTransparent(_pixels, i, j, channels))
				break;
		if (j == _height) --_trimed_region.xMax;
		else break;
	}

	const int width = _trimed_region.xLength(),
		height = _trimed_region.yLength();

	_trimed = (unsigned char*) malloc(width * height * channels);
	for (size_t i = 0; i < height; ++i) {
		const int from = ((_trimed_region.yMin+i) * _width + _trimed_region.xMin) * channels,
			to = ((height - 1 - i) * width) * channels;
		const int size = channels * width;
		memcpy(&_trimed[to], &_pixels[from], size);
	}

// 	std::string outpath = ee::FilenameTools::getFilePathExceptExtension(_filename) + "_trimed.png";
// 	stbi_write_png(outpath.c_str(), width, height, channels, _trimed, 0);

	stbi_write_png(_filename.c_str(), width, height, channels, _trimed, 0);
}

bool ExtrudeImg::IsTransparent(unsigned char* pixels, int x, int y, int channels)
{
	return _pixels[(_width * y + x) * channels + channels - 1] == 0;
}

void ExtrudeImg::Extrude(int extrude)
{
	int channels;
	int size = 0;
	switch(_format)
	{
	case 0x1907:	// GL_RGB
		channels = 3;
		return;
	case 0x1908:	// GL_RGBA
		channels = 4;
		break;
	default:
		return;
	}

	const int linesize = channels * _trimed_region.xLength();

	const int width = _trimed_region.xLength() + extrude * 2,
		height = _trimed_region.yLength() + extrude * 2;
	_extruded = (unsigned char*) malloc(width * height * channels);
	memset(_extruded, 0xff, width * height * channels);
	for (size_t i = 0, n = _trimed_region.yLength(); i < n; ++i) {
		const int from = (i * _trimed_region.xLength()) * channels,
			to = ((i + extrude) * width + extrude) * channels;
		memcpy(&_extruded[to], &_trimed[from], linesize);
	}

	// up
	for (size_t i = 0; i < extrude; ++i) {
		const int to = (i * width + extrude) * channels;
		memcpy(&_extruded[to], &_trimed[0], linesize);
	}
	// down
	for (size_t i = 0; i < extrude; ++i) {
		const int from = _trimed_region.xLength() * (_trimed_region.yLength() - 1) * channels,
			to = ((i + extrude + _trimed_region.yLength()) * width + extrude) * channels;
		memcpy(&_extruded[to], &_trimed[from], linesize);
	}
	// left
	for (size_t i = 0, n = _trimed_region.yLength(); i < n; ++i) {
		const int from = (_trimed_region.xLength() * i) * channels,
			to = (width * (i + extrude)) * channels;
		for (size_t j = 0; j < extrude; ++j)
			memcpy(&_extruded[to+j*channels], &_trimed[from], channels);
	}
	// right
	for (size_t i = 0, n = _trimed_region.yLength(); i < n; ++i) {
		const int from = (_trimed_region.xLength() * i + _trimed_region.xLength() - 1) * channels,
			to = (width * (i + extrude) + width - extrude) * channels;
		for (size_t j = 0; j < extrude; ++j)
			memcpy(&_extruded[to+j*channels], &_trimed[from], channels);
	}

// 	std::string outpath = ee::FilenameTools::getFilePathExceptExtension(_filename) + "_extrude.png";
// 	stbi_write_png(outpath.c_str(), width, height, channels, _extruded, 0);

	stbi_write_png(_filename.c_str(), width, height, channels, _extruded, 0);
}

}