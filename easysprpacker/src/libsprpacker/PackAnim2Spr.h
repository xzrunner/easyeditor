#ifndef _EASYSPRPACKER_PACK_ANIM2_SPR_H_
#define _EASYSPRPACKER_PACK_ANIM2_SPR_H_

#include "PackNode.h"

#include <memory>

namespace libanim2 { class Sprite; }

namespace esprpacker
{

class PackAnim2Spr : public PackNode
{
public:
	PackAnim2Spr(const std::shared_ptr<libanim2::Sprite>& spr);
	virtual ~PackAnim2Spr();

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
	
	bool Equal(const std::shared_ptr<libanim2::Sprite>& spr) const;

private:
	void Init(const std::shared_ptr<libanim2::Sprite>& spr);

private:
	const PackNode* m_sym;

	int m_static_time;

}; // PackAnim2Spr

}

#endif // _EASYSPRPACKER_PACK_ANIM2_SPR_H_