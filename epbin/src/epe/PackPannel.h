#ifndef _EPBIN_EPE_PACK_PANNEL_H_
#define _EPBIN_EPE_PACK_PANNEL_H_

#include "../IPackNode.h"

#include <vector>

struct lua_State;

namespace epbin
{
namespace epe
{

class PackPannel : public IPackNode
{
public:
	PackPannel(lua_State* L, uint32_t id);

	virtual size_t Size() const;
	virtual void Store(std::ofstream& fout) const;

private:
	void Load(lua_State* L);

private:
	struct Pannel
	{
		uint32_t width;
		uint32_t height;
		uint8_t scissor;
	};

private:
	Pannel m_pannel;

}; // PackPannel

}
}

#endif // _EPBIN_EPE_PACK_PANNEL_H_