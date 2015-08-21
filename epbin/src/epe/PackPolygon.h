#ifndef _EPBIN_EPE_PACK_POLYGON_H_
#define _EPBIN_EPE_PACK_POLYGON_H_

#include "../IPackNode.h"

#include <vector>

struct lua_State;

namespace epbin
{
namespace epe
{

class PackPolygon : public IPackNode
{
public:
	PackPolygon(lua_State* L, uint16_t id);

	virtual size_t Size() const;
	virtual void Store(std::ofstream& fout) const;

private:
	void Load(lua_State* L);

private:
	struct Poly
	{
		std::vector<uint16_t> texture_coord;
		std::vector<int32_t> screen_coord;
		int8_t texid;
	};

private:
	std::vector<Poly> m_polys;

}; // PackPolygon

}
}

#endif // _EPBIN_EPE_PACK_POLYGON_H_