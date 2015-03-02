#include "TransToETC1.h"
#include "ImagePack.h"

#include <rg_etc1.h>

namespace eimage
{

TransToETC1::TransToETC1(const uint8_t* pixels, int width, int height, int channels)
	: m_pixels(NULL)
	, m_alpha_pixels(NULL)
	, m_width(0)
	, m_height(0)
	, m_etc1_size(0)
	, m_etc1_pixels(NULL)
	, m_etc1_alpha_pixels(NULL)
{
	InitSrcImage(pixels, width, height, channels);
	InitSrcImageAlpha();

	InitETC1Header();
	EncodeETC1();
}

TransToETC1::~TransToETC1()
{
	delete[] m_pixels;
	delete[] m_alpha_pixels;
	delete[] m_etc1_pixels;
	delete[] m_etc1_alpha_pixels;
}

void TransToETC1::OutputFile(const std::string& filepath) const
{
	{
		std::string full_path = filepath + ".pkm";
		std::ofstream fout(full_path.c_str(), std::ios::binary);
		fout.write(reinterpret_cast<const char*>(&m_header), sizeof(m_header));
		fout.write(reinterpret_cast<const char*>(m_etc1_pixels), m_etc1_size);
		fout.close();
	}
	{
		std::string full_path = filepath + "_alpha.pkm";
		std::ofstream fout(full_path.c_str(), std::ios::binary);
		fout.write(reinterpret_cast<const char*>(&m_header), sizeof(m_header));
		fout.write(reinterpret_cast<const char*>(m_etc1_alpha_pixels), m_etc1_size);
		fout.close();
	}
}

uint8_t* TransToETC1::GetPixelsData(int& width, int& height) const
{
	width = m_width;
	height = m_height;

	uint8_t* pixels = new uint8_t[m_etc1_size * 2];
	memcpy(pixels, m_etc1_pixels, m_etc1_size);
	memcpy(pixels + m_etc1_size, m_etc1_alpha_pixels, m_etc1_size);
	return pixels;
}

void TransToETC1::InitSrcImage(const uint8_t* pixels, int width, int height, int channels)
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
		m_alpha_pixels = new uint8_t[sz];
		memset(m_alpha_pixels, 0, sz);
	}
	else
	{
		int nw = next_p2(width),
			nh = next_p2(height);
		ImagePack pack(nw, nh);
		pack.AddImage(pixels, width, height, 0, 0, ImagePack::PT_NORMAL);

		size_t sz = sizeof(uint8_t) * nw * nh * channels;
		m_pixels = new uint8_t[sz];
		memcpy(m_pixels, pack.GetPixels(), sz);
		m_alpha_pixels = new uint8_t[sz];
		memset(m_alpha_pixels, 0, sz);

		m_width = nw;
		m_height = nh;
	}
}

void TransToETC1::InitSrcImageAlpha()
{
	uint8_t *ptr_rgb = m_pixels,
		*ptr_alpha = m_alpha_pixels;
	for (int y = 0; y < m_height; ++y) {
		for (int x = 0; x < m_width; ++x) {
			ptr_alpha[0] = ptr_rgb[3];
			ptr_rgb[3] = ptr_alpha[3] = 255;

			ptr_alpha += 4;
			ptr_rgb += 4;
		}
	}
}

void TransToETC1::InitETC1Header()
{
	static const uint64_t pkm_identifier = 0x00003031204d4b50;	// "PKM 10"

	memcpy(m_header.identifier, &pkm_identifier, sizeof(uint8_t) * 8);
	m_header.widthLSB = m_header.paddedWidthLSB = (m_width & 0xff);
	m_header.widthMSB = m_header.paddedWidthMSB = (m_width >> 8);
	m_header.heightLSB = m_header.paddedHeightLSB = (m_height & 0xff);
	m_header.heightMSB = m_header.paddedHeightMSB = (m_height >> 8);
}

void TransToETC1::EncodeETC1()
{
	assert(m_width % 4 == 0 && m_height % 4 == 0);

	rg_etc1::pack_etc1_block_init();
	int bw = m_width / 4;
	int bh = m_height / 4;
	
	m_etc1_size = bw * bh * 8;
	m_etc1_pixels = new uint8_t[m_etc1_size];
	m_etc1_alpha_pixels = new uint8_t[m_etc1_size];

	rg_etc1::etc1_pack_params params;
	params.m_quality = rg_etc1::cLowQuality;	
	params.m_dithering = false;
	// rgb
	for (int y = 0; y < bh; ++y) {
		for (int x = 0; x < bw; ++x) {
			uint8_t block[64];
			for (int iy = 0; iy < 4; iy++) {
				memcpy(block + 16 * iy, m_pixels + ((y * 4 + iy) * m_width + x * 4) * 4, 16);
			}
			rg_etc1::pack_etc1_block(m_etc1_pixels + (bw * y + x) * 8, (const unsigned int*)block, params);
		}
	}
	// alpha
	for (int y = 0; y < bh; ++y) {
		for (int x = 0; x < bw; ++x) {
			uint8_t block[64];
			for (int iy = 0; iy < 4; iy++) {
				memcpy(block + 16 * iy, m_alpha_pixels + ((y * 4 + iy) * m_width + x * 4) * 4, 16);
			}
			rg_etc1::pack_etc1_block(m_etc1_alpha_pixels + (bw * y + x) * 8, (const unsigned int*)block, params);
		}
	}
}

}