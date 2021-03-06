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
	PackScale9(const std::shared_ptr<escale9::Symbol>& sym);

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
	void Init(const std::shared_ptr<escale9::Symbol>& sym);

private:
	struct Grid
	{
		const PackNode* node;
		float angle;
		sm::bvec2 mirror;

		Grid() : node(NULL), angle(0), mirror(false, false) {}
		~Grid() { if (node) { node->RemoveReference(); }  }
	};

private:
	s2::SCALE9_TYPE m_type;

//	Grid m_grids[9];
	std::vector<Grid> m_grids;

	int m_left, m_right, m_top, m_down;

}; // PackScale9

}

#endif // _EASYSPRPACKER_PACK_SCALE9_H_