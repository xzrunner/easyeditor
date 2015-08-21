#ifndef _EPBIN_BINARY_EPE_H_
#define _EPBIN_BINARY_EPE_H_

#include <stdint.h>
#include <vector>
#include <map>

struct lua_State;

namespace epbin
{

class IPackNode;

class BinaryEPE
{
public:
	BinaryEPE(const std::string& infile);
	~BinaryEPE();

	void Pack(const std::string& outfile, bool compress);

private:
	void Load(const std::string& infile);

	void PackImpl(const std::string& outfile, bool compress);

private:
	struct Block
	{
		uint8_t* data;
		uint32_t len;
	};

	Block PackNodes2Mem(const std::string& outfile) const;

private:
	int m_texture, m_maxid;

	std::map<std::string, int> m_map_export;

	std::vector<IPackNode*> m_nodes;

}; // BinaryEPE

}

#endif // _EPBIN_BINARY_EPE_H_