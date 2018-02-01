#ifndef _EASYRESPACKER_PACK_SHAPE_H_
#define _EASYRESPACKER_PACK_SHAPE_H_

#include "IPackNode.h"

#include <cu/cu_stl.h>
#include <SM_Vector.h>
#include <painting2/Color.h>

namespace erespacker
{

enum ShapeType
{
	ST_POINTS = 0,
	ST_LINES,
	ST_LINE_LOOP,
	ST_LINE_STRIP,
	ST_TRIANGLES,
	ST_TRIANGLE_STRIP,
	ST_TRIANGLE_FAN
};

class PackShape : public IPackNode
{
public:
	ShapeType type;

	pt2::Color color;

	CU_VEC<sm::vec2> vertices;

public:
	PackShape() {}
	PackShape(int id);

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

}; // PackShape

}

#endif // _EASYRESPACKER_PACK_SHAPE_H_