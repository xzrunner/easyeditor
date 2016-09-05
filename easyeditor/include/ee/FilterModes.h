#ifndef _EASYEDITOR_FILTER_MODES_H_
#define _EASYEDITOR_FILTER_MODES_H_

#include <CU_Singleton.h>

#include <sprite2/FilterMode.h>

#include <map>
#include <vector>
#include <string>

namespace ee
{

class FilterModes
{
public:
	s2::FilterMode ID2Mode(int idx) const;
	int Mode2ID(s2::FilterMode mode) const;

	void GetAllNameCN(std::vector<std::string>& names) const;

private:
	struct Item
	{
		Item(s2::FilterMode id, const std::string& name)
			: id(id), name(name) {}

		s2::FilterMode id;
		std::string name;
	};

private:
	std::map<s2::FilterMode, int> m_map2idx;

	std::vector<Item> m_modes;

	SINGLETON_DECLARATION(FilterModes);

}; // FilterModes

}

#endif // _EASYEDITOR_FILTER_MODES_H_