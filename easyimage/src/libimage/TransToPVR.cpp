#include "TransToPVR.h"
#include "ImageIO.h"
#include "ImagePack.h"

#include <PVRTextureUtilities.h>

namespace eimage
{

struct PVRTexHeader {
	uint32_t headerLength;
	uint32_t height;
	uint32_t width;
	uint32_t numMipmaps;
	uint32_t flags;
	uint32_t dataLength;
	uint32_t bpp;
	uint32_t bitmaskRed;
	uint32_t bitmaskGreen;
	uint32_t bitmaskBlue;
	uint32_t bitmaskAlpha;
	uint32_t pvrTag;
	uint32_t numSurfs;
};

void TransToPVR::Trans(const std::string& filepath)
{
	int w, h, c, f;
	uint8_t* src_buf = ImageIO::Read(filepath.c_str(), w, h, c, f);
	if (!is_power_of_two(w) || !is_power_of_two(h) || w != h) {
		int nw = next_p2(w),
			nh = next_p2(h);
		nw = nh = std::max(nw, nh);
		ImagePack pack(nw, nh);
		pack.AddImage(src_buf, w, h, 0, 0, ImagePack::PT_NORMAL);
		uint8_t* pixels = pack.GetPixels();
		
		size_t sz = sizeof(uint8_t) * nw * nh * 4;
		uint8_t* new_src_buf = new uint8_t[sz];
		memcpy(new_src_buf, pack.GetPixels(), sz);

		w = nw;
		h = nh;

		delete[] src_buf;
		src_buf = new_src_buf;
	}
	
	PVRTexHeader raw_header;
	memset(&raw_header, 0, sizeof(raw_header));
	raw_header.headerLength = PVRTEX3_HEADERSIZE;
	raw_header.width = w;
	raw_header.height = h;
	raw_header.flags = 32793;
	raw_header.dataLength = w * h * 0.5f;
	raw_header.bpp = 4;
	raw_header.bitmaskAlpha = 1;
	raw_header.pvrTag = 559044176;
	raw_header.numSurfs = 1;

	pvrtexture::CPVRTextureHeader header;
	header.setWidth(w);
	header.setHeight(h);
	header.setNumMIPLevels(1);
	header.setPixelFormat(pvrtexture::PixelType('r','g','b','a',8,8,8,8));

	pvrtexture::CPVRTexture texture = pvrtexture::CPVRTexture(header, src_buf);
//	pvrtexture::ECompressorQuality quality = pvrtexture::ePVRTCFastest;
	pvrtexture::ECompressorQuality quality = pvrtexture::ePVRTCBest;
	bool suc = Transcode(texture, pvrtexture::PixelType(ePVRTPF_PVRTCI_4bpp_RGBA), texture.getChannelType(), texture.getColourSpace(), quality, false);
	assert(suc);
	
	size_t dst_sz = texture.getDataSize(PVRTEX_ALLMIPLEVELS, false, false);
	uint8_t* dst_buf = new uint8_t[dst_sz];
	memcpy(dst_buf, texture.getDataPtr(0, 0, 0), dst_sz);

	std::string dst_path = filepath.substr(0, filepath.find_last_of('.')) + ".pvr";
	std::ofstream fout(dst_path.c_str(), std::ios::binary);
	fout.write(reinterpret_cast<const char*>(&raw_header), sizeof(raw_header));
	fout.write(reinterpret_cast<const char*>(dst_buf), dst_sz);
	fout.close();

	delete[] dst_buf;
}

}