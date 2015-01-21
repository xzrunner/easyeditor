#include "TransToPVR.h"
#include "ImageIO.h"

namespace eimage
{

void TransToPVR::Trans(const std::string& filepath)
{
	int w, h, c, f;
	uint8_t* src_buf = ImageIO::Read(filepath.c_str(), w, h, c, f);
	
}

}