#ifndef _LIBRESPACKER_PACK_CLIPBOX_H_
#define _LIBRESPACKER_PACK_CLIPBOX_H_

#include "IPackNode.h"

#include <drag2d.h>

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
		const d2d::TexturePacker& tp, float scale) const;
	virtual void UnpackFromLua(lua_State* L,
		const std::vector<d2d::Image*>& images);

	virtual int SizeOfPackToBin() const;
	virtual void PackToBin(uint8_t** ptr,
		const d2d::TexturePacker& tp, float scale) const;
	virtual int SizeOfUnpackFromBin() const;
	virtual void UnpackFromBin(uint8_t** ptr, 
		const std::vector<d2d::Image*>& images);

}; // PackClipbox

}

#endif // _LIBRESPACKER_PACK_CLIPBOX_H_