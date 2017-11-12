#ifndef _EASYSPRPACKER_PACK_PARTICLE3D_SPR_H_
#define _EASYSPRPACKER_PACK_PARTICLE3D_SPR_H_

#include "PackNode.h"

#include <sprite2/Particle3dSprite.h>

#include <memory>

namespace eparticle3d { class Sprite; }

namespace esprpacker
{

class PackParticle3dSpr : public PackNode
{
public:
	PackParticle3dSpr(const std::shared_ptr<eparticle3d::Sprite>& spr);
	virtual ~PackParticle3dSpr();

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

	bool Equal(const std::shared_ptr<eparticle3d::Sprite>& spr) const;

private:
	void Init(const std::shared_ptr<eparticle3d::Sprite>& spr);

private:
	const PackNode* m_sym;

	bool m_loop;
	bool m_local;
	bool m_alone;
	s2::Particle3dSprite::ReuseType m_reuse;

	float m_start_radius;

}; // PackParticle3dSpr

}

#endif // _EASYSPRPACKER_PACK_PARTICLE3D_SPR_H_