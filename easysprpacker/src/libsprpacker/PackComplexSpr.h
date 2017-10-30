#ifndef _EASYSPRPACKER_PACK_COMPLEX_SPR_H_
#define _EASYSPRPACKER_PACK_COMPLEX_SPR_H_

#include "PackNode.h"

#include <memory>

namespace ecomplex { class Sprite; }

namespace esprpacker
{

class PackComplexSpr : public PackNode
{
public:
	PackComplexSpr(const std::shared_ptr<ecomplex::Sprite>& spr);
	virtual ~PackComplexSpr();

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

	bool Equal(const std::shared_ptr<ecomplex::Sprite>& spr) const;

private:
	void Init(const std::shared_ptr<ecomplex::Sprite>& spr);

private:
	const PackNode* m_sym;
	
	int m_action;

}; // PackComplexSpr

}

#endif // _EASYSPRPACKER_PACK_COMPLEX_SPR_H_