#ifndef _EASYSPRPACKER_PACK_SHAPE_H_
#define _EASYSPRPACKER_PACK_SHAPE_H_

#include "PackNode.h"

#include <sprite2/MaskSymbol.h>
#include <painting2/Color.h>
#include <s2loader/ShapeType.h>
#include <cu/cu_stl.h>

#include <vector>
#include <memory>

namespace eshape { class Symbol; }

namespace esprpacker
{

class PackShape : public PackNode
{
public:
	PackShape(const std::shared_ptr<eshape::Symbol>& sym);
	virtual ~PackShape();

	/**
	 *  @interface
	 *    lua
	 */
	virtual void PackToLuaString(ebuilder::CodeGenerator& gen,
		const ee::TexturePacker& tp) const override;
	
	/**
	 *  @interface
	 *    bin
	 */
	virtual int SizeOfUnpackFromBin() const override;
	virtual int SizeOfPackToBin() const override;
	virtual void PackToBin(uint8_t** ptr, const ee::TexturePacker& tp) const override;	

private:
	s2loader::ShapeType m_type;

	pt2::Color m_color;
	const PackNode* m_texture;

	CU_VEC<sm::vec2> m_vertices;

	bool m_filling;

}; // PackShape

}

#endif // _EASYSPRPACKER_PACK_SHAPE_H_