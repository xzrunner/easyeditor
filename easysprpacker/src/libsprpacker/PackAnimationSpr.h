#ifndef _EASYSPRPACKER_PACK_ANIMATION_SPR_H_
#define _EASYSPRPACKER_PACK_ANIMATION_SPR_H_

#include "PackNode.h"

namespace libanim { class Sprite; }

namespace esprpacker
{

class PackAnimationSpr : public PackNode
{
public:
	PackAnimationSpr(const libanim::Sprite* spr);
	~PackAnimationSpr();

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

	bool Equal(const libanim::Sprite* spr) const;

private:
	void Init(const libanim::Sprite* spr);

private:
	const PackNode* m_sym;

	bool m_loop;
	float m_interval;

	int m_fps;

}; // PackAnimationSpr

}

#endif // _EASYSPRPACKER_PACK_ANIMATION_SPR_H_