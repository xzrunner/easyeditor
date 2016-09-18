#ifndef _EASYSPRPACKER_PACK_ANIMATION_SPR_H_
#define _EASYSPRPACKER_PACK_ANIMATION_SPR_H_

#include "PackNode.h"

namespace eanim { class Sprite; }

namespace esprpacker
{

class PackAnimationSpr : public PackNode
{
public:
	PackAnimationSpr(const eanim::Sprite* spr);

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

	bool GetLoop() const { return m_loop; }
	float GetInterval() const { return m_interval; }
	int GetFPS() const { return m_fps; }

private:
	void Init(const eanim::Sprite* spr);

private:
	const PackNode* m_sym;

	bool m_loop;
	float m_interval;

	int m_fps;

}; // PackAnimationSpr

}

#endif // _EASYSPRPACKER_PACK_ANIMATION_SPR_H_