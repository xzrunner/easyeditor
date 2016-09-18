#ifndef _EASYSPRPACKER_PACK_PARTICLE3D_SPR_H_
#define _EASYSPRPACKER_PACK_PARTICLE3D_SPR_H_

#include "PackNode.h"

namespace eparticle3d { class Sprite; }

namespace esprpacker
{

class PackParticle3dSpr : public PackNode
{
public:
	PackParticle3dSpr(const eparticle3d::Sprite* spr);

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
	bool IsAlone() const { return m_alone; }
	bool IsReuse() const { return m_reuse; }

private:
	void Init(const eparticle3d::Sprite* spr);

private:
	const PackNode* m_sym;

	bool m_loop;
	bool m_local;
	bool m_alone;
	bool m_reuse;

}; // PackParticle3dSpr

}

#endif // _EASYSPRPACKER_PACK_PARTICLE3D_SPR_H_