#ifndef _EASYSPRPACKER_PACK_MASK_SPR_H_
#define _EASYSPRPACKER_PACK_MASK_SPR_H_

#include "PackNode.h"

#include <memory>

namespace emask { class Sprite; }

namespace esprpacker
{

class PackMaskSpr : public PackNode
{
public:
	PackMaskSpr(const std::shared_ptr<emask::Sprite>& spr);
	virtual ~PackMaskSpr();

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

	bool Equal(const std::shared_ptr<emask::Sprite>& spr) const { return true; }

private:
	void Init(const std::shared_ptr<emask::Sprite>& spr);

private:
	const PackNode* m_sym;

}; // PackMaskSpr

}

#endif // _EASYSPRPACKER_PACK_MASK_SPR_H_