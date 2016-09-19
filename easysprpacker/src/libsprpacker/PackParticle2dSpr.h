#ifndef _EASYSPRPACKER_PACK_PARTICLE2D_SPR_H_
#define _EASYSPRPACKER_PACK_PARTICLE2D_SPR_H_

#include "PackNode.h"

namespace eparticle2d { class Sprite; }

namespace esprpacker
{

class PackParticle2dSpr : public PackNode
{
public:
	PackParticle2dSpr(const eparticle2d::Sprite* spr);

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

	bool IsLoop() const { return m_loop; }
	bool IsLocal() const { return m_local; }

private:
	void Init(const eparticle2d::Sprite* spr);

private:
	const PackNode* m_sym;

	bool m_loop;
	bool m_local;

}; // PackParticle2dSpr

}

#endif // _EASYSPRPACKER_PACK_PARTICLE2D_SPR_H_