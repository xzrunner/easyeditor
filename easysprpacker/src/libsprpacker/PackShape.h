#ifndef _EASYSPRPACKER_PACK_SHAPE_H_
#define _EASYSPRPACKER_PACK_SHAPE_H_

#include "PackNode.h"

#include <sprite2/MaskSymbol.h>
#include <sprite2/Color.h>
#include <gum/ShapeType.h>

#include <vector>

namespace eshape { class Symbol; }

namespace esprpacker
{

class PackShape : public PackNode
{
public:
	PackShape(const eshape::Symbol* sym);

	/**
	 *  @interface
	 *    lua
	 */
	virtual void PackToLuaString(ebuilder::CodeGenerator& gen,
		const ee::TexturePacker& tp, float scale) const;
	
	/**
	 *  @interface
	 *    bin
	 */
	virtual int SizeOfUnpackFromBin() const;
	virtual int SizeOfPackToBin() const;
	virtual void PackToBin(uint8_t** ptr, const ee::TexturePacker& tp, 
		float scale) const;	

private:
	gum::ShapeType m_type;

	s2::Color m_color;
	const PackNode* m_texture;

	std::vector<sm::vec2> m_vertices;

	bool m_filling;

}; // PackShape

}

#endif // _EASYSPRPACKER_PACK_SHAPE_H_