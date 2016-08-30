#ifndef _EASYSPRPACKER_PACK_ICON_H_
#define _EASYSPRPACKER_PACK_ICON_H_

#include "PackNode.h"

namespace eicon { class Symbol; }

namespace esprpacker
{

class PackIcon : public PackNode
{
public:
	PackIcon(const eicon::Symbol* sym);

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

private:
	void Init(const eicon::Symbol* sym);

private:
	int m_type;

}; // PackIcon

}

#endif // _EASYSPRPACKER_PACK_ICON_H_