#ifndef _EASYSPRPACKER_PACK_ANIM2_SPR_H_
#define _EASYSPRPACKER_PACK_ANIM2_SPR_H_

#include "PackNode.h"

namespace libanim2 { class Sprite; }

namespace esprpacker
{

class PackAnim2Spr : public PackNode
{
public:
	PackAnim2Spr(const libanim2::Sprite* spr);
	virtual ~PackAnim2Spr();

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
	
	bool Equal(const libanim2::Sprite* spr) const;

private:
	void Init(const libanim2::Sprite* spr);

private:
	const PackNode* m_sym;

	int m_static_time;

}; // PackAnim2Spr

}

#endif // _EASYSPRPACKER_PACK_ANIM2_SPR_H_