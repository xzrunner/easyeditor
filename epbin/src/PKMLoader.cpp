#include "PKMLoader.h"
#include "Exception.h"
#include "Lzma.h"
#include "image_type.h"

#include <assert.h>

namespace epbin
{

struct PKMHeader
{
	unsigned char identifier[8];
	unsigned char paddedWidthMSB;
	unsigned char paddedWidthLSB;
	unsigned char paddedHeightMSB;
	unsigned char paddedHeightLSB;
	unsigned char widthMSB;
	unsigned char widthLSB;
	unsigned char heightMSB;
	unsigned char heightLSB;
};

PKMLoader::PKMLoader()
{
	m_type = TYPE_PKMC;

	m_width = m_height = 0;
	m_rgb_buf = m_alpha_buf = NULL;
}

PKMLoader::~PKMLoader()
{
	delete m_rgb_buf;
	delete m_alpha_buf;
}

void PKMLoader::Load(const std::string& filepath)
{
	std::string filename = filepath.substr(0, filepath.find_last_of("."));

	LoadCompressed(filename+".pkm", m_rgb_buf, m_width, m_height);

	int w, h;
	LoadCompressed(filename+"_alpha.pkm", m_alpha_buf, w, h);	
	assert(w == m_width && h == m_height);
}

void PKMLoader::Store(std::ofstream& fout) const
{
	if (m_compress)
	{
		size_t tex_sz = (m_width * m_height) >> 1;
		size_t sz = sizeof(int8_t) + sizeof(int16_t) * 2 + tex_sz * 2;
		uint8_t* buf = new uint8_t[sz];
		uint8_t* ptr = buf;

		memcpy(ptr, &m_type, sizeof(int8_t));
		ptr += sizeof(int8_t);
		memcpy(ptr, &m_width, sizeof(int16_t));
		ptr += sizeof(int16_t);
		memcpy(ptr, &m_height, sizeof(int16_t));
		ptr += sizeof(int16_t);
		memcpy(ptr, m_rgb_buf, tex_sz);
		ptr += tex_sz;
		memcpy(ptr, m_alpha_buf, tex_sz);
		ptr += tex_sz;

		uint8_t* dst = NULL;
		size_t dst_sz;
		Lzma::Compress(&dst, &dst_sz, buf, sz);

		delete[] buf;

		fout.write(reinterpret_cast<const char*>(&dst_sz), sizeof(uint32_t));
		fout.write(reinterpret_cast<const char*>(dst), dst_sz);
		delete[] dst;
	}
	else
	{
		int img_sz = (m_width * m_height) >> 1;

		int sz = 0;
		sz += sizeof(int8_t);	// type
		sz += sizeof(int16_t);	// width
		sz += sizeof(int16_t);	// height
		sz += img_sz * 2;
		sz = -sz;
		fout.write(reinterpret_cast<const char*>(&sz), sizeof(int32_t));

		fout.write(reinterpret_cast<const char*>(&m_type), sizeof(int8_t));
		fout.write(reinterpret_cast<const char*>(&m_width), sizeof(int16_t));
		fout.write(reinterpret_cast<const char*>(&m_height), sizeof(int16_t));

		fout.write(reinterpret_cast<const char*>(m_rgb_buf), img_sz);
		fout.write(reinterpret_cast<const char*>(m_alpha_buf), img_sz);		
	}
}

void PKMLoader::LoadCompressed(const std::string& filename, uint8_t*& buf, 
							   int& width, int& height)
{
	std::ifstream fin(filename.c_str(), std::ios::binary);

	PKMHeader header;
	fin.read(reinterpret_cast<char*>(&header), sizeof(header));

	width = (header.paddedWidthMSB << 8) | header.paddedWidthLSB;
	height = (header.paddedHeightMSB << 8) | header.paddedHeightLSB;
	uint32_t sz = (width * height) >> 1;
	buf = new uint8_t[sz];
	if (!buf) {
		throw Exception("Out of memory: PKMLoader::LoadCompressed %s \n", filename.c_str());
	}
	fin.read(reinterpret_cast<char*>(buf), sz);

	fin.close();
}

}