#ifndef _LIBRESPACKER_PACK_ANCHOR_H_
#define _LIBRESPACKER_PACK_ANCHOR_H_

#include "IPackNode.h"

#include <spritepack.h>



namespace librespacker
{

class PackAnchor : public IPackNode
{
public:
	PackAnchor() { m_spr_id = ANCHOR_ID; }
	PackAnchor(int id) { m_spr_id = ANCHOR_ID; }

	virtual void PackToLuaString(ebuilder::CodeGenerator& gen,
		const ee::TexturePacker& tp, float scale) const {}
	virtual void UnpackFromLua(lua_State* L,
		const std::vector<ee::Image*>& images) {}

	virtual int SizeOfPackToBin() const { return 0; }
	virtual void PackToBin(uint8_t** ptr,
		const ee::TexturePacker& tp, float scale) const {}
	virtual int SizeOfUnpackFromBin() const { return 0; }
	virtual void UnpackFromBin(uint8_t** ptr, 
		const std::vector<ee::Image*>& images) {}

}; // PackAnchor 

}

#endif // _LIBRESPACKER_PACK_ANCHOR_H_