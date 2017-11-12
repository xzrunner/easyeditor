#ifndef _EASYSPRPACKER_PACK_MASK_H_
#define _EASYSPRPACKER_PACK_MASK_H_

#include "PackNode.h"

#include <sprite2/MaskSymbol.h>

#include <vector>
#include <memory>

namespace emask { class Symbol; }

namespace esprpacker
{

class PackMask : public PackNode
{
public:
	PackMask(const std::shared_ptr<emask::Symbol>& sym);
	~PackMask();

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
	const PackNode *m_base, *m_mask;

}; // PackMask

}

#endif // _EASYSPRPACKER_PACK_MASK_H_