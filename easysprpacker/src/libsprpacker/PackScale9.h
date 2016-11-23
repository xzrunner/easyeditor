#ifndef _EASYSPRPACKER_PACK_SCALE9_H_
#define _EASYSPRPACKER_PACK_SCALE9_H_

#include "PackNode.h"

#include <sprite2/Scale9.h>

#include <SM_Vector.h>

namespace escale9 { class Symbol; }

namespace esprpacker
{

class PackScale9 : public PackNode
{
public:
	PackScale9(const escale9::Symbol* sym);
	virtual ~PackScale9();

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
	void Init(const escale9::Symbol* sym);

private:
	struct Grid
	{
		const PackNode* node;
		float angle;
		sm::bvec2 mirror;

		Grid() : node(NULL), angle(0), mirror(false, false) {}
	};

private:
	s2::SCALE9_TYPE m_type;

//	Grid m_grids[9];
	std::vector<Grid> m_grids;

}; // PackScale9

}

#endif // _EASYSPRPACKER_PACK_SCALE9_H_