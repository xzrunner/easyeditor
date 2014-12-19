#ifndef _EPBIN_BINARY_EPD_H_
#define _EPBIN_BINARY_EPD_H_

#include <fstream>

struct lua_State;

namespace epbin
{

class BinaryEPD
{
public:
	BinaryEPD(const std::string& src, const std::string& dst);
	~BinaryEPD();

	void Pack();

private:
	void CheckID(int id);

	void BinPic(lua_State* L, int id);
	void BinAni(lua_State* L, int id);

private:
	std::string m_src_filepath;

	int m_max_id;
	int m_export;

	std::ofstream m_fout;

}; // BinEP

}

#endif // _EPBIN_BINARY_EPD_H_