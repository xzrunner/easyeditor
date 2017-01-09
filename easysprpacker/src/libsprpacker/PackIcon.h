#ifndef _EASYSPRPACKER_PACK_ICON_H_
#define _EASYSPRPACKER_PACK_ICON_H_

#include "PackNode.h"

#include <gum/IconType.h>
#include <SM_Vector.h>

#include <vector>

namespace eicon { class Symbol; }

namespace esprpacker
{

class PackIcon : public PackNode
{
public:
	PackIcon(const eicon::Symbol* sym);
	virtual ~PackIcon();

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
	const PackNode* m_base;

	gum::IconType m_type;

	std::vector<sm::vec2> m_vertices;

}; // PackIcon

}

#endif // _EASYSPRPACKER_PACK_ICON_H_