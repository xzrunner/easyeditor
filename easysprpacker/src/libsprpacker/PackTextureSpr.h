#ifndef _EASYSPRPACKER_PACK_TEXTURE_SPR_H_
#define _EASYSPRPACKER_PACK_TEXTURE_SPR_H_

#include "PackNode.h"

namespace etexture { class Sprite; }

namespace esprpacker
{

class PackTextureSpr : public PackNode
{
public:
	PackTextureSpr(const etexture::Sprite* spr);

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

	bool Equal(const etexture::Sprite* spr) const { return true; }

private:
	const PackNode* m_sym;

}; // PackTextureSpr

}

#endif // _EASYSPRPACKER_PACK_TEXTURE_SPR_H_