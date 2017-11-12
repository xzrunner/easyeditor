#ifndef _EASYRESPACKER_PACK_CLIPBOX_H_
#define _EASYRESPACKER_PACK_CLIPBOX_H_

#include "IPackNode.h"



namespace erespacker
{

class PackClipbox : public IPackNode
{
public:
	int x, y;
	int w, h;

public:
	PackClipbox() {}
	PackClipbox(int id);

	virtual void PackToLuaString(ebuilder::CodeGenerator& gen,
		const ee::TexturePacker& tp, float scale) const override;
	virtual void UnpackFromLua(lua_State* L,
		const std::vector<ee::ImagePtr>& images) override;

	virtual int SizeOfPackToBin() const override;
	virtual void PackToBin(uint8_t** ptr,
		const ee::TexturePacker& tp, float scale) const override;
	virtual int SizeOfUnpackFromBin() const override;
	virtual void UnpackFromBin(uint8_t** ptr, 
		const std::vector<ee::ImagePtr>& images) override;

}; // PackClipbox

}

#endif // _EASYRESPACKER_PACK_CLIPBOX_H_