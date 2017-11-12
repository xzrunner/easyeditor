#ifndef _EASYSPRPACKER_PACK_TEXTURE_SPR_H_
#define _EASYSPRPACKER_PACK_TEXTURE_SPR_H_

#include "PackNode.h"

#include <memory>

namespace etexture { class Sprite; }

namespace esprpacker
{

class PackTextureSpr : public PackNode
{
public:
	PackTextureSpr(const std::shared_ptr<etexture::Sprite>& spr);
	virtual ~PackTextureSpr();

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

	bool Equal(const std::shared_ptr<etexture::Sprite>& spr) const { return true; }

private:
	const PackNode* m_sym;

}; // PackTextureSpr

}

#endif // _EASYSPRPACKER_PACK_TEXTURE_SPR_H_