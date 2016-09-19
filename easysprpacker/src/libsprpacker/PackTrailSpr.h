#ifndef _EASYSPRPACKER_PACK_TRAIL_SPR_H_
#define _EASYSPRPACKER_PACK_TRAIL_SPR_H_

#include "PackNode.h"

namespace etrail { class Sprite; }

namespace esprpacker
{

class PackTrailSpr : public PackNode
{
public:
	PackTrailSpr(const etrail::Sprite* spr);

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
	void Init(const etrail::Sprite* spr);

private:
	const PackNode* m_sym;

}; // PackTrailSpr

}

#endif // _EASYSPRPACKER_PACK_TRAIL_SPR_H_