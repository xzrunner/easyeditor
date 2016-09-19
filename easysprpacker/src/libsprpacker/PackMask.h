#ifndef _EASYSPRPACKER_PACK_MASK_H_
#define _EASYSPRPACKER_PACK_MASK_H_

#include "PackNode.h"

#include <sprite2/MaskSymbol.h>

#include <vector>

namespace emask { class Symbol; }

namespace esprpacker
{

class PackMask : public PackNode
{
public:
	PackMask(const emask::Symbol* sym);
	~PackMask();

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
	const PackNode *m_base, *m_mask;

}; // PackMask

}

#endif // _EASYSPRPACKER_PACK_MASK_H_