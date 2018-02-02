#ifndef _S2LOADER_FILTER_MODES_H_
#define _S2LOADER_FILTER_MODES_H_

#include <cu/cu_macro.h>
#include <cu/cu_stl.h>
#include <painting2/FilterMode.h>

namespace s2loader
{

class FilterModes
{
public:
	pt2::FilterMode Name2Mode(const CU_STR& name) const;
	CU_STR Mode2Name(pt2::FilterMode id) const;

	int QueryShaderIdx(pt2::FilterMode mode) const;

private:
	struct Item
	{
		Item(pt2::FilterMode id, const CU_STR& name)
			: id(id), name(name){}

		pt2::FilterMode id;
		CU_STR name;
	};

private:
	CU_MAP<pt2::FilterMode, int> m_map2idx;

	CU_VEC<Item> m_modes;

	CU_SINGLETON_DECLARATION(FilterModes);

}; // FilterModes

}

#endif // _S2LOADER_FILTER_MODES_H_
