#include "TransToPVR.h"
#include "ImageIO.h"

#include <PVRTextureUtilities.h>

namespace eimage
{

void TransToPVR::Trans(const std::string& filepath)
{
	int w, h, c, f;
	uint8_t* src_buf = ImageIO::Read(filepath.c_str(), w, h, c, f);
	
	pvrtexture::CPVRTextureHeader header;
	header.setWidth(w);
	header.setHeight(h);
	header.setNumMIPLevels(1);
	header.setPixelFormat(pvrtexture::PixelType('r','g','b','a',8,8,8,8));

	pvrtexture::CPVRTexture texture = pvrtexture::CPVRTexture(header, src_buf);

	texture.getChannelType();

// 	bool suc = Transcode(texture, pvrtexture::PixelType(ePVRTPF_PVRTCI_4bpp_RGBA), texture.getChannelType(), texture.getColourSpace(), pvrtexture::ePVRTCNormal, false);
// 	assert(suc);
	
//	size_t dst_sz = texture.getDataSize(PVRTEX_ALLMIPLEVELS, false, false);
	size_t dst_sz = 0;
	uint8_t* dst_buf = new uint8_t[dst_sz];
	memcpy(dst_buf, texture.getDataPtr(0, 0, 0), dst_sz);

	std::string dst_path = filepath.substr(0, filepath.find_last_of('.')) + ".pvr";
	std::ofstream fout(dst_path.c_str(), std::ios::binary);
	fout.write(reinterpret_cast<const char*>(dst_buf), dst_sz);
	fout.close();

	delete[] dst_buf;
}

}