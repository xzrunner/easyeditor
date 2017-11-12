#ifndef _EASYSPRPACKER_PACK_TEXTURE_H_
#define _EASYSPRPACKER_PACK_TEXTURE_H_

#include "PackNode.h"

#include <vector>
#include <memory>

namespace etexture { class Symbol; }

namespace esprpacker
{

class PackTexture : public PackNode
{
public:
	PackTexture(const std::shared_ptr<etexture::Symbol>& sym);
	virtual ~PackTexture();

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

private:
	std::vector<const PackNode*> m_polys;

}; // PackTexture

}

#endif // _EASYSPRPACKER_PACK_TEXTURE_H_