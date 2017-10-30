#ifndef _EASYSPRPACKER_PACK_TRAIL_SPR_H_
#define _EASYSPRPACKER_PACK_TRAIL_SPR_H_

#include "PackNode.h"

#include <memory>

namespace etrail { class Sprite; }

namespace esprpacker
{

class PackTrailSpr : public PackNode
{
public:
	PackTrailSpr(const std::shared_ptr<etrail::Sprite>& spr);
	virtual ~PackTrailSpr();

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

	bool Equal(const std::shared_ptr<etrail::Sprite>& spr) const { return true; }

private:
	const PackNode* m_sym;

}; // PackTrailSpr

}

#endif // _EASYSPRPACKER_PACK_TRAIL_SPR_H_