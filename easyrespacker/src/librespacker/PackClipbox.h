#ifndef _LIBRESPACKER_PACK_CLIPBOX_H_
#define _LIBRESPACKER_PACK_CLIPBOX_H_

#include "IPackNode.h"



namespace librespacker
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
		const ee::TexturePacker& tp, float scale) const;
	virtual void UnpackFromLua(lua_State* L,
		const std::vector<ee::Image*>& images);

	virtual int SizeOfPackToBin() const;
	virtual void PackToBin(uint8_t** ptr,
		const ee::TexturePacker& tp, float scale) const;
	virtual int SizeOfUnpackFromBin() const;
	virtual void UnpackFromBin(uint8_t** ptr, 
		const std::vector<ee::Image*>& images);

}; // PackClipbox

}

#endif // _LIBRESPACKER_PACK_CLIPBOX_H_