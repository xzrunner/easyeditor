#include "ImageSaver.h"
#include "common/Exception.h"

#include "LibpngAdapter.h"
#include "PPMAdapter.h"

namespace d2d
{

void ImageSaver::StoreToFile(const uint8_t* pixels, int width, int height,
							 int channels, const std::string& filename, Type type)
{
	if (!pixels) {
		throw Exception("Fail to store image %s: null pixles!", filename.c_str());
	}

	switch (type)
	{
	case e_png:
		LibpngAdapter::Write(pixels, width, height, filename);
		break;
	case e_ppm:
		PPMAdapter::Write(pixels, width, height, filename);
		break;
	case e_gif:
		break;
	}
}

}