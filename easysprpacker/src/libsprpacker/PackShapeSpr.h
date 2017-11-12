#ifndef _EASYSPRPACKER_PACK_SHAPE_SPR_H_
#define _EASYSPRPACKER_PACK_SHAPE_SPR_H_

#include "PackNode.h"

#include <memory>

namespace eshape { class Sprite; }

namespace esprpacker
{

class PackShapeSpr : public PackNode
{
public:
	PackShapeSpr(const std::shared_ptr<eshape::Sprite>& spr);
	virtual ~PackShapeSpr();

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

	bool Equal(const std::shared_ptr<eshape::Sprite>& spr) const { return true; }

private:
	const PackNode* m_sym;

}; // PackShapeSpr

}

#endif // _EASYSPRPACKER_PACK_SHAPE_SPR_H_