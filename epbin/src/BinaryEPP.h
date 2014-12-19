#ifndef _EPBIN_BINARY_EPP_H_
#define _EPBIN_BINARY_EPP_H_

#include <fstream>

namespace epbin
{

enum TextureType
{
	TT_PNG4,
	TT_PNG8,
	TT_PVR,
	TT_PKM
};

class BinaryEPP
{
public:
	BinaryEPP(const std::string& dir, const std::string& filename, TextureType type);

	void Pack();

private:
	std::vector<std::string> m_src_files;

}; // BinEPP

}

#endif // _EPBIN_BINARY_EPP_H_