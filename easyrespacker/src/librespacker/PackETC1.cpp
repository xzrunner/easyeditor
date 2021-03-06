#include "PackETC1.h"
#include "Lzma.h"
#include "typedef.h"

#include <ee/Exception.h>

#include <timp/TextureFormat.h>

#include <string>

#include <assert.h>

namespace erespacker
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

PackETC1::PackETC1()
{
	m_type = timp::TEXTURE_ETC1;

	m_width = m_height = 0;
	m_rgb_buf = m_alpha_buf = NULL;
}

PackETC1::~PackETC1()
{
	Clear();
}

void PackETC1::Load(const std::string& filepath)
{
	Clear();

	std::string filename = filepath.substr(0, filepath.find_last_of("."));
	LoadCompressed(filename + ".pkm", m_rgb_buf, m_width, m_height);

	int w, h;
	LoadCompressed(filename + "_alpha.pkm", m_alpha_buf, w, h);	
	assert(w == m_width && h == m_height);
}

void PackETC1::Store(const std::string& filepath, float scale) const
{
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str(), std::ios::binary);
	std::locale::global(std::locale("C"));

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

		if (dst_sz > 0) {
			fout.write(reinterpret_cast<const char*>(&dst_sz), sizeof(uint32_t));
			fout.write(reinterpret_cast<const char*>(dst), dst_sz);
			delete[] dst;
		}
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

	fout.close();
}

void PackETC1::Clear()
{
	delete m_rgb_buf, m_rgb_buf = NULL;
	delete m_alpha_buf, m_alpha_buf = NULL;
}

void PackETC1::LoadCompressed(const std::string& filename, uint8_t*& buf, 
							   int& width, int& height)
{
	std::locale::global(std::locale(""));	
	std::ifstream fin(filename.c_str(), std::ios::binary);
	std::locale::global(std::locale("C"));

	PKMHeader header;
	fin.read(reinterpret_cast<char*>(&header), sizeof(header));

	width = (header.paddedWidthMSB << 8) | header.paddedWidthLSB;
	height = (header.paddedHeightMSB << 8) | header.paddedHeightLSB;
	uint32_t sz = (width * height) >> 1;
	buf = new uint8_t[sz];
	if (!buf) {
		throw ee::Exception("Out of memory: PackETC1::LoadCompressed %s \n", filename.c_str());
	}
	fin.read(reinterpret_cast<char*>(buf), sz);

	fin.close();
}

}