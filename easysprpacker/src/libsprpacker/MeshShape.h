#ifndef _EASYSPRPACKER_MESH_SHAPE_H_
#define _EASYSPRPACKER_MESH_SHAPE_H_

#include <stdint.h>

namespace ebuilder { class CodeGenerator; }

namespace esprpacker
{

class MeshShape
{
public:
	virtual ~MeshShape() {}
	virtual int Type() const = 0;
	virtual void PackToLuaString(ebuilder::CodeGenerator& gen) const = 0;
	virtual int SizeOfUnpackFromBin() const = 0;
	virtual int SizeOfPackToBin() const = 0;
	virtual void PackToBin(uint8_t** ptr) const = 0;
}; // MeshShape

}

#endif // _EASYSPRPACKER_MESH_SHAPE_H_