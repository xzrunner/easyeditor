#ifndef _LIBRESPACKER_PACK_LABEL_H_
#define _LIBRESPACKER_PACK_LABEL_H_

#include "IPackNode.h"



namespace librespacker
{

class PackLabel : public IPackNode
{
public:
	int width;
	int height;

	int font;
	int font_size;
	ee::Colorf font_color;

	bool edge;
	float edge_size;
	ee::Colorf edge_color;

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
		const ee::TexturePacker& tp, float scale) const;
	virtual void UnpackFromLua(lua_State* L,
		const std::vector<ee::Image*>& images);

	virtual int SizeOfPackToBin() const;
	virtual void PackToBin(uint8_t** ptr,
		const ee::TexturePacker& tp, float scale) const;
	virtual int SizeOfUnpackFromBin() const;
	virtual void UnpackFromBin(uint8_t** ptr, 
		const std::vector<ee::Image*>& images);

}; // PackLabel

}

#endif // _LIBRESPACKER_PACK_LABEL_H_