#ifndef _EASYSPRPACKER_PACK_SCALE9_SPR_H_
#define _EASYSPRPACKER_PACK_SCALE9_SPR_H_

#include "PackNode.h"

#include <SM_Vector.h>

#include <memory>

namespace escale9 { class Sprite; }

namespace esprpacker
{

class PackScale9Spr : public PackNode
{
public:
	PackScale9Spr(const std::shared_ptr<escale9::Sprite>& spr);

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

	bool Equal(const std::shared_ptr<escale9::Sprite>& spr) const;

private:
	void Init(const std::shared_ptr<escale9::Sprite>& spr);

private:
	const PackNode* m_sym;

	float m_width, m_height;

}; // PackScale9Spr

}

#endif // _EASYSPRPACKER_PACK_SCALE9_SPR_H_