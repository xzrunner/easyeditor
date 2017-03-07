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
		const ee::TexturePacker& tp, float scale) const;
	
	/**
	 *  @interface
	 *    bin
	 */
	virtual int SizeOfUnpackFromBin() const;
	virtual int SizeOfPackToBin() const;
	virtual void PackToBin(uint8_t** ptr, const ee::TexturePacker& tp, 
		float scale) const;

}; // PackAnchor

}

#endif // _EASYSPRPACKER_PACK_ANCHOR_H_