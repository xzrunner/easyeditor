#ifndef _EASYRESPACKER_PACK_TRAIL_H_
#define _EASYRESPACKER_PACK_TRAIL_H_

#include "IPackNode.h"

#include <SM_Vector.h>
#include <sprite2/Color.h>

namespace erespacker
{

class PackTrail : public IPackNode
{
public:
	struct CompImage
	{
		const IPackNode* node;
		float scale_begin, scale_end;
		uint32_t mul_col_begin, mul_col_end;
		uint32_t add_col_begin, add_col_end;
	}; // CompImage

	struct CompShape
	{
		float linewidth;
		float acuity;
		uint32_t col_begin, col_end;
	}; // CompShape

	int mode;

	int count;

	float life_begin;
	float life_offset;

	float fadeout_time;

	std::vector<CompImage> comp_images;
	std::vector<CompShape> comp_shapes;

public:	
	PackTrail() {}
	PackTrail(int id);

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

}; // PackTrail

}

#endif // _EASYRESPACKER_PACK_TRAIL_H_