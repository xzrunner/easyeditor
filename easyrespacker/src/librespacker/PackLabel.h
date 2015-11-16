#ifndef _LIBRESPACKER_PACK_LABEL_H_
#define _LIBRESPACKER_PACK_LABEL_H_

#include "IPackNode.h"

#include <drag2d.h>

namespace librespacker
{

class PackLabel : public IPackNode
{
public:
	int width;
	int height;

	int font;
	int font_size;
	d2d::Colorf font_color;

	bool edge;
	float edge_size;
	d2d::Colorf edge_color;

	int align_hori;
	int align_vert;

	float space_hori;
	float space_vert;

	std::string text;
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