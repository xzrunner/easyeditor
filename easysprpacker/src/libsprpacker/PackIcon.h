#ifndef _EASYSPRPACKER_PACK_ICON_H_
#define _EASYSPRPACKER_PACK_ICON_H_

#include "PackNode.h"

#include <gum/IconType.h>
#include <SM_Vector.h>
#include <cu/cu_stl.h>

#include <vector>
#include <memory>

namespace eicon { class Symbol; }

namespace esprpacker
{

class PackIcon : public PackNode
{
public:
	PackIcon(const std::shared_ptr<eicon::Symbol>& sym);
	virtual ~PackIcon();

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
	const PackNode* m_base;

	gum::IconType m_type;

	CU_VEC<sm::vec2> m_vertices;

}; // PackIcon

}

#endif // _EASYSPRPACKER_PACK_ICON_H_