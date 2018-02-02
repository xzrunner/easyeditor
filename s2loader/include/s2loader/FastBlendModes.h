#ifndef _S2LOADER_FAST_BLEND_MODES_H_
#define _S2LOADER_FAST_BLEND_MODES_H_

#include <cu/cu_macro.h>
#include <cu/cu_stl.h>
#include <painting2/FastBlendMode.h>

namespace s2loader
{

class FastBlendModes
{
public:
	pt2::FastBlendMode Name2Mode(const CU_STR& name) const;
	CU_STR Mode2Name(pt2::FastBlendMode mode) const;

private:
	 struct Item
	 {
		Item(pt2::FastBlendMode mode, const CU_STR& name)
			: mode(mode), name(name) {}

	 	pt2::FastBlendMode mode;
	 	CU_STR name;
	 };

private:
	CU_VEC<Item> m_modes;

	CU_SINGLETON_DECLARATION(FastBlendModes);

}; // FastBlendModes

}

#endif // _S2LOADER_FAST_BLEND_MODES_H_
