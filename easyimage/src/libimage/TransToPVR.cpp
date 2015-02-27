#include "TransToPVR.h"
#include "ImageIO.h"
#include "ImagePack.h"

#include <PVRTextureUtilities.h>

namespace eimage
{

TransToPVR::TransToPVR(const uint8_t* pixels, int width, int height, int channels)
	: m_pixels(NULL)
	, m_width(0)
	, m_height(0)
	, m_pvr_size(0)
	, m_pvr_pixels(NULL)
{
	InitSrcImage(pixels, width, height, channels);
	InitPVRHeader();
	TransPVR();
}

TransToPVR::~TransToPVR()
{
	delete[] m_pixels;
	delete[] m_pvr_pixels;
}

void TransToPVR::OutputFile(const std::string& filepath) const
{
	std::ofstream fout(filepath.c_str(), std::ios::binary);
	fout.write(reinterpret_cast<const char*>(&m_header), sizeof(m_header));
	fout.write(reinterpret_cast<const char*>(m_pvr_pixels), m_pvr_size);
	fout.close();
}

uint8_t* TransToPVR::GetPVRData(int& width, int& height) const
{
	width = m_width;
	height = m_height;
	return m_pvr_pixels;
}

void TransToPVR::InitSrcImage(const uint8_t* pixels, int width, int height, int channels)
{
	assert(channels == 4);
	if (is_power_of_two(width) &&
		is_power_of_two(height) &&
		width == height) 
	{
		m_width = width;
		m_height = height;

		size_t sz = sizeof(uint8_t) * width * height * channels;
		m_pixels = new uint8_t[sz];
		memcpy(m_pixels, pixels, sz);
	}
	else
	{
		int nw = next_p2(width),
			nh = next_p2(height);
		nw = nh = std::max(nw, nh);
		ImagePack pack(nw, nh);
		pack.AddImage(pixels, width, height, 0, nh - height, ImagePack::PT_NORMAL);

		size_t sz = sizeof(uint8_t) * nw * nh * channels;
		m_pixels = new uint8_t[sz];
		memcpy(m_pixels, pack.GetPixels(), sz);

		m_width = nw;
		m_height = nh;
	}
}

void TransToPVR::InitPVRHeader()
{
	memset(&m_header, 0, sizeof(m_header));
	m_header.headerLength = PVRTEX3_HEADERSIZE;
	m_header.width = m_width;
	m_header.height = m_height;
	m_header.flags = 32793;
	m_header.dataLength = m_width * m_height * 0.5f;
	m_header.bpp = 4;
	m_header.bitmaskAlpha = 1;
	m_header.pvrTag = 559044176;
	m_header.numSurfs = 1;
}

void TransToPVR::TransPVR()
{
	pvrtexture::CPVRTextureHeader header;
	header.setWidth(m_width);
	header.setHeight(m_height);
	header.setNumMIPLevels(1);
	header.setPixelFormat(pvrtexture::PixelType('r','g','b','a',8,8,8,8));

	pvrtexture::CPVRTexture texture = pvrtexture::CPVRTexture(header, m_pixels);
//	pvrtexture::ECompressorQuality quality = pvrtexture::ePVRTCFastest;
	pvrtexture::ECompressorQuality quality = pvrtexture::ePVRTCBest;
	bool suc = Transcode(texture, pvrtexture::PixelType(ePVRTPF_PVRTCI_4bpp_RGBA), texture.getChannelType(), texture.getColourSpace(), quality, false);
	assert(suc);

	m_pvr_size = texture.getDataSize(PVRTEX_ALLMIPLEVELS, false, false);
	m_pvr_pixels = new uint8_t[m_pvr_size];
	memcpy(m_pvr_pixels, texture.getDataPtr(0, 0, 0), m_pvr_size);
}

}