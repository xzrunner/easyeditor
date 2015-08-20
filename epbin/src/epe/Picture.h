#ifndef _EPBIN_EPE_PICTURE_H_
#define _EPBIN_EPE_PICTURE_H_

#include "../INode.h"

#include <vector>

struct lua_State;

namespace epbin
{
namespace epe
{

class Picture : public INode
{
public:
	Picture(lua_State* L);

	virtual size_t Size() const;

	virtual void Store(uint8_t** ptr);

private:
	struct Quad
	{
		int texid;
		uint16_t texture_coord[8];
		int32_t screen_coord[8];
	};

private:
	void Load(lua_State* L);

private:
	std::vector<Quad> m_quads;

}; // Picture

}
}

#endif // _EPBIN_EPE_PICTURE_H_