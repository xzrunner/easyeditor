#ifndef _EPBIN_BINARY_EPD_H_
#define _EPBIN_BINARY_EPD_H_

#include <vector>

#include <stdint.h>

struct lua_State;

namespace epbin
{

namespace epd
{
	class Picture;
	class Animation;
}

class BinaryEPD
{
public:
	BinaryEPD(const std::string& infile);
	~BinaryEPD();

	void Pack(const std::string& outfile, bool compress);

private:
	void Load(const std::string& infile);

	void PackImpl(const std::string& outfile, bool compress);

	void CheckID(int id);
	void CheckExport(lua_State* L);

private:
	uint16_t m_max_id;
	uint16_t m_export;

	std::vector<epd::Picture*> m_pictures;
	std::vector<epd::Animation*> m_animations;

}; // BinaryEPD

}

#endif // _EPBIN_BINARY_EPD_H_