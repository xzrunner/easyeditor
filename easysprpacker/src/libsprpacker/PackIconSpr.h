#ifndef _EASYSPRPACKER_PACK_ICON_SPR_H_
#define _EASYSPRPACKER_PACK_ICON_SPR_H_

#include "PackNode.h"

namespace eicon { class Sprite; }

namespace esprpacker
{

class PackIconSpr : public PackNode
{
public:
	PackIconSpr(const eicon::Sprite* spr);

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

	bool Equal(const eicon::Sprite* spr) const;

private:	
	const PackNode* m_sym;

	float m_process;

}; // PackIconSpr

}

#endif // _EASYSPRPACKER_PACK_ICON_SPR_H_