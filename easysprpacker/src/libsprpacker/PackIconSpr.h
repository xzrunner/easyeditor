#ifndef _EASYSPRPACKER_PACK_ICON_SPR_H_
#define _EASYSPRPACKER_PACK_ICON_SPR_H_

#include "PackNode.h"

#include <memory>

namespace eicon { class Sprite; }

namespace esprpacker
{

class PackIconSpr : public PackNode
{
public:
	PackIconSpr(const std::shared_ptr<eicon::Sprite>& spr);
	virtual ~PackIconSpr();

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

	bool Equal(const eicon::Sprite* spr) const;

private:	
	const PackNode* m_sym;

	float m_process;

}; // PackIconSpr

}

#endif // _EASYSPRPACKER_PACK_ICON_SPR_H_