#ifndef _EPBIN_TEX_LOADER_H_
#define _EPBIN_TEX_LOADER_H_

#include <fstream>

namespace epbin
{

class TexLoader
{
public:
	TexLoader();
	virtual ~TexLoader();

	virtual void Load(const std::string& filepath) = 0;

	void Store(std::ofstream& fout) const;

protected:
	int m_type;
	int m_width, m_height;

	unsigned char* m_buffer;
	int m_buf_sz;

}; // TexLoader

}

#endif // _EPBIN_TEX_LOADER_H_