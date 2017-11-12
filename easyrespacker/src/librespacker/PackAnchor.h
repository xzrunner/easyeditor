#ifndef _EASYRESPACKER_PACK_ANCHOR_H_
#define _EASYRESPACKER_PACK_ANCHOR_H_

#include "IPackNode.h"

namespace erespacker
{

class PackAnchor : public IPackNode
{
public:
	PackAnchor();
	PackAnchor(int id);

	virtual void PackToLuaString(ebuilder::CodeGenerator& gen,
		const ee::TexturePacker& tp, float scale) const override {}
	virtual void UnpackFromLua(lua_State* L,
		const std::vector<ee::ImagePtr>& images) override {}

	virtual int SizeOfPackToBin() const override { return 0; }
	virtual void PackToBin(uint8_t** ptr,
		const ee::TexturePacker& tp, float scale) const override {}
	virtual int SizeOfUnpackFromBin() const override { return 0; }
	virtual void UnpackFromBin(uint8_t** ptr, 
		const std::vector<ee::ImagePtr>& images) override {}

}; // PackAnchor 

}

#endif // _EASYRESPACKER_PACK_ANCHOR_H_