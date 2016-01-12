#ifndef _LIBRESPACKER_PACK_SHAPE_H_
#define _LIBRESPACKER_PACK_SHAPE_H_

#include "IPackNode.h"

#include <drag2d.h>

namespace librespacker
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

	d2d::Colorf color;

	std::vector<d2d::Vector> vertices;

public:
	PackShape() {}
	PackShape(int id);

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

}; // PackShape

}

#endif // _LIBRESPACKER_PACK_SHAPE_H_