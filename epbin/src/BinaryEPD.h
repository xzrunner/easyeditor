#ifndef _EPBIN_BINARY_EPD_H_
#define _EPBIN_BINARY_EPD_H_

#include "typedef.h"

#include <fstream>
#include <vector>

struct lua_State;

namespace epbin
{

class Picture;
class Animation;

class BinaryEPD
{
public:
	BinaryEPD(const std::string& src, const std::string& dst);
	~BinaryEPD();

	void Pack(bool compress);

private:
	void Load();

	void CheckID(int id);
	void CheckExport(lua_State* L);

private:
	std::string m_src_filepath;

	std::ofstream m_fout;

	uint16_t m_max_id;
	uint16_t m_export;

	std::vector<Picture*> m_pictures;
	std::vector<Animation*> m_animations;

}; // BinEP

}

#endif // _EPBIN_BINARY_EPD_H_