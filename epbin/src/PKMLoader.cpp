#include "PKMLoader.h"
#include "Exception.h"
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
	LoadCompressed(filepath+".pkm", m_rgb_buf, m_width, m_height);

	int w, h;
	LoadCompressed(filepath+"_alpha.pkm", m_alpha_buf, w, h);	
	assert(w == m_width && h == m_height);
}

void PKMLoader::Store(std::ofstream& fout) const
{
	fout.write(reinterpret_cast<const char*>(&m_type), sizeof(int8_t));
	fout.write(reinterpret_cast<const char*>(&m_width), sizeof(int16_t));
	fout.write(reinterpret_cast<const char*>(&m_height), sizeof(int16_t));

	int sz = (m_width * m_height) >> 1;
	fout.write(reinterpret_cast<const char*>(m_rgb_buf), sz);
	fout.write(reinterpret_cast<const char*>(m_alpha_buf), sz);
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