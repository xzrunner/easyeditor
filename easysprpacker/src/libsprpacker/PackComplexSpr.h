#ifndef _EASYSPRPACKER_PACK_COMPLEX_SPR_H_
#define _EASYSPRPACKER_PACK_COMPLEX_SPR_H_

#include "PackNode.h"

namespace ecomplex { class Sprite; }

namespace esprpacker
{

class PackComplexSpr : public PackNode
{
public:
	PackComplexSpr(const ecomplex::Sprite* spr);
	virtual ~PackComplexSpr();

	/**
	 *  @interface
	 *    lua
	 */
	virtual void PackToLuaString(ebuilder::CodeGenerator& gen,
		const ee::TexturePacker& tp, float scale) const;
	
	/**
	 *  @interface
	 *    bin
	 */
	virtual int SizeOfUnpackFromBin() const;
	virtual int SizeOfPackToBin() const;
	virtual void PackToBin(uint8_t** ptr, const ee::TexturePacker& tp, 
		float scale) const;

	bool Equal(const ecomplex::Sprite* spr) const;

private:
	void Init(const ecomplex::Sprite* spr);

private:
	const PackNode* m_sym;
	
	int m_action;

}; // PackComplexSpr

}

#endif // _EASYSPRPACKER_PACK_COMPLEX_SPR_H_