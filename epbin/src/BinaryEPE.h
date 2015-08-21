#ifndef _EPBIN_BINARY_EPE_H_
#define _EPBIN_BINARY_EPE_H_

#include <stdint.h>
#include <vector>

struct lua_State;

namespace epbin
{

class BinaryEPE
{
public:
	BinaryEPE(const std::string& infile);
	~BinaryEPE();

	void Pack(const std::string& outfile, bool compress);

private:
	void Load(const std::string& infile);

	void PackImpl(const std::string& outfile, bool compress);

}; // BinaryEPE

}

#endif // _EPBIN_BINARY_EPE_H_