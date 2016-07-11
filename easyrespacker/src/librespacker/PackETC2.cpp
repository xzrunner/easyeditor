#include "PackETC2.h"
#include "Lzma.h"
#include "typedef.h"

#include <ee/Exception.h>

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

PackETC2::PackETC2()
{
	m_type = TT_ETC2;

	m_width = m_height = 0;
	m_buf = NULL;
}

PackETC2::~PackETC2()
{
	Clear();
}

void PackETC2::Load(const std::string& filepath)
{
	Clear();

	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str(), std::ios::binary);
	std::locale::global(std::locale("C"));

	PKMHeader header;
	fin.read(reinterpret_cast<char*>(&header), sizeof(header));

	m_width = (header.paddedWidthMSB << 8) | header.paddedWidthLSB;
	m_height = (header.paddedHeightMSB << 8) | header.paddedHeightLSB;
	uint32_t sz = m_width * m_height;
	m_buf = new uint8_t[sz];
	if (!m_buf) {
		throw ee::Exception("Out of memory: PackETC2::LoadCompressed %s \n", filepath.c_str());
	}
	fin.read(reinterpret_cast<char*>(m_buf), sz);

	fin.close();
}

void PackETC2::Store(const std::string& filepath, float scale) const
{
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str(), std::ios::binary);
	std::locale::global(std::locale("C"));

	size_t body_sz = m_width * m_height;
	if (m_compress)
	{
		size_t tot_size = sizeof(int8_t) + sizeof(int16_t) * 2 + body_sz;
		uint8_t* buf = new uint8_t[tot_size];
		uint8_t* ptr = buf;

		memcpy(ptr, &m_type, sizeof(int8_t));
		ptr += sizeof(int8_t);
		memcpy(ptr, &m_width, sizeof(int16_t));
		ptr += sizeof(int16_t);
		memcpy(ptr, &m_height, sizeof(int16_t));
		ptr += sizeof(int16_t);
		memcpy(ptr, m_buf, body_sz);
		ptr += body_sz;

		uint8_t* dst = NULL;
		size_t dst_sz;
		Lzma::Compress(&dst, &dst_sz, buf, tot_size);

		delete[] buf;

		if (dst_sz > 0) {
			fout.write(reinterpret_cast<const char*>(&dst_sz), sizeof(uint32_t));
			fout.write(reinterpret_cast<const char*>(dst), dst_sz);
			delete[] dst;
		}
	}
	else
	{
		int body_sz = (m_width * m_height) >> 1;

		int sz = 0;
		sz += sizeof(int8_t);	// type
		sz += sizeof(int16_t);	// width
		sz += sizeof(int16_t);	// height
		sz += body_sz;
		sz = -sz;
		fout.write(reinterpret_cast<const char*>(&sz), sizeof(int32_t));

		fout.write(reinterpret_cast<const char*>(&m_type), sizeof(int8_t));
		fout.write(reinterpret_cast<const char*>(&m_width), sizeof(int16_t));
		fout.write(reinterpret_cast<const char*>(&m_height), sizeof(int16_t));

		fout.write(reinterpret_cast<const char*>(m_buf), body_sz);
	}

	fout.close();
}

void PackETC2::Clear()
{
	delete m_buf, m_buf = NULL;
}

}