#ifndef _EASYSPRPACKER_PACK_ANCHOR_H_
#define _EASYSPRPACKER_PACK_ANCHOR_H_

#include "PackNode.h"

namespace esprpacker
{

class PackAnchor : public PackNode
{
public:
	PackAnchor();

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

}; // PackAnchor

}

#endif // _EASYSPRPACKER_PACK_ANCHOR_H_