#ifndef _EASYSPRPACKER_PACK_ANIMATION_SPR_H_
#define _EASYSPRPACKER_PACK_ANIMATION_SPR_H_

#include "PackNode.h"

#include <memory>

namespace libanim { class Sprite; }

namespace esprpacker
{

class PackAnimationSpr : public PackNode
{
public:
	PackAnimationSpr(const std::shared_ptr<libanim::Sprite>& spr);
	~PackAnimationSpr();

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

	bool Equal(const std::shared_ptr<libanim::Sprite>& spr) const;

private:
	void Init(const std::shared_ptr<libanim::Sprite>& spr);

private:
	const PackNode* m_sym;

	bool m_loop;
	float m_interval;

	bool m_start_random;

	int m_fps;

}; // PackAnimationSpr

}

#endif // _EASYSPRPACKER_PACK_ANIMATION_SPR_H_