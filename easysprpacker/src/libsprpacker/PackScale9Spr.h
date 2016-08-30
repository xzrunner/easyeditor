#ifndef _EASYSPRPACKER_PACK_SCALE9_SPR_H_
#define _EASYSPRPACKER_PACK_SCALE9_SPR_H_

#include "PackNode.h"

namespace escale9 { class Sprite; }

namespace esprpacker
{

class PackScale9Spr : public PackNode
{
public:
	PackScale9Spr(const escale9::Sprite* spr);

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
	void Init(const escale9::Sprite* spr);

private:
	float m_width, m_height;

}; // PackScale9Spr

}

#endif // _EASYSPRPACKER_PACK_SCALE9_SPR_H_