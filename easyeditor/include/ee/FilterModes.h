#ifndef _EASYEDITOR_FILTER_MODES_H_
#define _EASYEDITOR_FILTER_MODES_H_

#include <cu/cu_macro.h>

#include <painting2/FilterMode.h>

#include <map>
#include <vector>
#include <string>

namespace ee
{

class FilterModes
{
public:
	pt2::FilterMode ID2Mode(int idx) const;
	int Mode2ID(pt2::FilterMode mode) const;

	void GetAllNameCN(std::vector<std::string>& names) const;

private:
	struct Item
	{
		Item(pt2::FilterMode id, const std::string& name)
			: id(id), name(name) {}

		pt2::FilterMode id;
		std::string name;
	};

private:
	std::map<pt2::FilterMode, int> m_map2idx;

	std::vector<Item> m_modes;

	CU_SINGLETON_DECLARATION(FilterModes);

}; // FilterModes

}

#endif // _EASYEDITOR_FILTER_MODES_H_