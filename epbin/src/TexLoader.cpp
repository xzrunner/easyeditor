#include "TexLoader.h"
#include "typedef.h"

namespace epbin
{

TexLoader::TexLoader()
	: m_buffer(NULL)
	, m_buf_sz(0)
{
}

TexLoader::~TexLoader()
{
	delete[] m_buffer;
}

void TexLoader::Store(std::ofstream& fout) const
{
	fout.write(reinterpret_cast<const char*>(&m_type), sizeof(int8_t));
	fout.write(reinterpret_cast<const char*>(&m_width), sizeof(int16_t));
	fout.write(reinterpret_cast<const char*>(&m_height), sizeof(int16_t));
	
	// todo different
	fout.write(reinterpret_cast<const char*>(m_buffer), sizeof(m_buf_sz));
}

}