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
	BinaryEPP(const std::string& dir, const std::string& filename, TextureType type,
		const std::string& output);

	void Pack() const;

private:
	std::string m_dir;
	std::string m_filename;
	TextureType m_type;

	std::string m_output;

}; // BinEPP

}

#endif // _EPBIN_BINARY_EPP_H_