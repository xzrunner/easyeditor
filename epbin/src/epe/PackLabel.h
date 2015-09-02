#ifndef _EPBIN_EPE_PACK_LABEL_H_
#define _EPBIN_EPE_PACK_LABEL_H_

#include "../IPackNode.h"

#include <vector>

struct lua_State;

namespace epbin
{
namespace epe
{

class PackLabel : public IPackNode
{
public:
	PackLabel(lua_State* L, uint32_t id);

	virtual size_t Size() const;
	virtual void Store(std::ofstream& fout) const;

private:
	void Load(lua_State* L);

private:
	struct Label
	{
		uint8_t align;
		uint32_t color;
		uint16_t size;
		uint16_t width;
		uint16_t height;
		uint8_t edge;
		uint8_t space_h;
		uint8_t space_w;
		uint8_t auto_scale;
	};

private:
	Label m_label;

}; // PackLabel

}
}

#endif // _EPBIN_EPE_LABEL_H_