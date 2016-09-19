#ifndef _EASYSPRPACKER_PACK_MASK_SPR_H_
#define _EASYSPRPACKER_PACK_MASK_SPR_H_

#include "PackNode.h"

namespace emask { class Sprite; }

namespace esprpacker
{

class PackMaskSpr : public PackNode
{
public:
	PackMaskSpr(const emask::Sprite* spr);

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

	bool Equal(const emask::Sprite* spr) const { return true; }

private:
	void Init(const emask::Sprite* spr);

private:
	const PackNode* m_sym;

}; // PackMaskSpr

}

#endif // _EASYSPRPACKER_PACK_MASK_SPR_H_