#ifndef _EPBIN_TEX_LOADER_H_
#define _EPBIN_TEX_LOADER_H_

#include <fstream>

namespace epbin
{

class TexLoader
{
public:
	TexLoader() : m_compress(true) {}
	virtual ~TexLoader() {}

	virtual void Load(const std::string& filepath) = 0;
	virtual void Store(std::ofstream& fout) const = 0;

protected:
	int m_type;

	bool m_compress;

}; // TexLoader

}

#endif // _EPBIN_TEX_LOADER_H_