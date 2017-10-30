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
		const ee::TexturePacker& tp) const;
	
	/**
	 *  @interface
	 *    bin
	 */
	virtual int SizeOfUnpackFromBin() const;
	virtual int SizeOfPackToBin() const;
	virtual void PackToBin(uint8_t** ptr, const ee::TexturePacker& tp) const;	

	bool Equal(const std::shared_ptr<eshape::Sprite>& spr) const { return true; }

private:
	const PackNode* m_sym;

}; // PackShapeSpr

}

#endif // _EASYSPRPACKER_PACK_SHAPE_SPR_H_