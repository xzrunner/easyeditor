#ifndef _S2LOADER_BLEND_MODES_H_
#define _S2LOADER_BLEND_MODES_H_

#include <cu/cu_macro.h>
#include <cu/cu_stl.h>
#include <painting2/BlendMode.h>

namespace s2loader
{

class BlendModes
{
public:
	pt2::BlendMode Name2Mode(const CU_STR& name) const;
	CU_STR ModeToName(pt2::BlendMode mode) const;

private:
	 struct Item
	 {
		Item(pt2::BlendMode mode, const CU_STR& name)
			: mode(mode), name(name) {}

	 	pt2::BlendMode mode;
	 	CU_STR name;
	 };

private:
	CU_VEC<Item> m_modes;

	CU_SINGLETON_DECLARATION(BlendModes);

}; // BlendModes

}

#endif // _S2LOADER_BLEND_MODES_H_