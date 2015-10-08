#ifndef _LIBCOCO_PACK_ANCHOR_H_
#define _LIBCOCO_PACK_ANCHOR_H_

#include "IPackNode.h"
#include "spritepack.h"

#include <drag2d.h>

namespace libcoco
{

class PackAnchor : public IPackNode
{
public:
	PackAnchor() { m_id = ANCHOR_ID; }
	PackAnchor(int id) { m_id = ANCHOR_ID; }

	virtual void PackToLuaString(ebuilder::CodeGenerator& gen,
		const d2d::TexturePacker& tp) const {}
	virtual void UnpackFromLua(lua_State* L,
		const std::vector<d2d::Image*>& images) {}

	virtual int SizeOfPackToBin() const { return 0; }
	virtual void PackToBin(uint8_t** ptr,
		const d2d::TexturePacker& tp) const {}
	virtual int SizeOfUnpackFromBin() const { return 0; }
	virtual void UnpackFromBin(uint8_t** ptr, 
		const std::vector<d2d::Image*>& images) {}

}; // PackAnchor 

}

#endif // _LIBCOCO_PACK_ANCHOR_H_