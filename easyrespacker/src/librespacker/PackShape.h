#ifndef _EASYRESPACKER_PACK_SHAPE_H_
#define _EASYRESPACKER_PACK_SHAPE_H_

#include "IPackNode.h"

#include <ee/Color.h>

#include <SM_Vector.h>

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

	ee::Colorf color;

	std::vector<sm::vec2> vertices;

public:
	PackShape() {}
	PackShape(int id);

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

}; // PackShape

}

#endif // _EASYRESPACKER_PACK_SHAPE_H_