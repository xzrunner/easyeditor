#ifndef _EASYSPRPACKER_PACK_PARTICLE2D_SPR_H_
#define _EASYSPRPACKER_PACK_PARTICLE2D_SPR_H_

#include "PackNode.h"

#include <memory>

namespace eparticle2d { class Sprite; }

namespace esprpacker
{

class PackParticle2dSpr : public PackNode
{
public:
	PackParticle2dSpr(const std::shared_ptr<eparticle2d::Sprite>& spr);
	virtual ~PackParticle2dSpr();

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

	bool Equal(const std::shared_ptr<eparticle2d::Sprite>& spr) const;

private:
	void Init(const std::shared_ptr<eparticle2d::Sprite>& spr);

private:
	const PackNode* m_sym;

	bool m_loop;
	bool m_local;

}; // PackParticle2dSpr

}

#endif // _EASYSPRPACKER_PACK_PARTICLE2D_SPR_H_