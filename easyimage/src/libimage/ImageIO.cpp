#include "ImageIO.h"

#include "LibpngAdapter.h"

namespace eimage
{

int ImageIO::Write(const uint8_t* pixels, int width, int height, const char* filename)
{
	return LibpngAdapter::Write(pixels, width, height, filename);	
}

uint8_t* ImageIO::Read(const char* filepath, int& width, int& height, int& channels, int& format)
{
	return LibpngAdapter::Read(filepath, width, height, channels, format);
}

void ImageIO::ReadHeader(const char* filepath, int& width, int& height)
{
	
}

}