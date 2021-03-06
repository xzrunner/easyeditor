#ifndef _EASYRESPACKER_PACK_LABEL_H_
#define _EASYRESPACKER_PACK_LABEL_H_

#include "IPackNode.h"

#include <painting2/Color.h>

namespace erespacker
{

class PackLabel : public IPackNode
{
public:
	int width;
	int height;

	int font;
	int font_size;
	pt2::Color font_color;

	bool edge;
	float edge_size;
	pt2::Color edge_color;

	int align_hori;
	int align_vert;

	float space_hori;
	float space_vert;

	bool overflow;

	bool richtext;

	std::string text;
	std::string tid;

public:
	PackLabel() {}
	PackLabel(int id);

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

}; // PackLabel

}

#endif // _EASYRESPACKER_PACK_LABEL_H_