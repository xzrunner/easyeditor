#ifndef _LIBRESPACKER_PACK_LABEL_H_
#define _LIBRESPACKER_PACK_LABEL_H_

#include "IPackNode.h"

#include <drag2d.h>

namespace librespacker
{

class PackLabel : public IPackNode
{
public:
	std::string name;
	std::string font;

	int align_hori;
	int align_vert;

	d2d::Colorf color;	// PT_ARGB

	int size;
	int width, height;

	bool has_edge;

	std::string content;
	std::string tid;

public:
	PackLabel() {}
	PackLabel(int id);

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

}; // PackLabel

}

#endif // _LIBRESPACKER_PACK_LABEL_H_