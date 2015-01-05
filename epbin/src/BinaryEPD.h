#ifndef _EPBIN_BINARY_EPD_H_
#define _EPBIN_BINARY_EPD_H_

#include "typedef.h"

#include <vector>

struct lua_State;

namespace epbin
{

class Picture;
class Animation;

class BinaryEPD
{
public:
	BinaryEPD(const std::string& infile);
	~BinaryEPD();

	void Pack(const std::string& outfile, bool compress);

private:
	void Load(const std::string& infile);

	void CheckID(int id);
	void CheckExport(lua_State* L);

private:
	uint16_t m_max_id;
	uint16_t m_export;

	std::vector<Picture*> m_pictures;
	std::vector<Animation*> m_animations;

}; // BinaryEPD

}

#endif // _EPBIN_BINARY_EPD_H_