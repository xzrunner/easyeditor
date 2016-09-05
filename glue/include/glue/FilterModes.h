#ifndef _GLUE_FILTER_MODES_H_
#define _GLUE_FILTER_MODES_H_

#include <CU_Singleton.h>

#include <sprite2/FilterMode.h>

#include <map>
#include <vector>
#include <string>

namespace glue
{

class FilterModes
{
public:
	s2::FilterMode Name2Mode(const std::string& name) const;
	std::string Mode2Name(s2::FilterMode id) const;

	int QueryShaderIdx(s2::FilterMode mode) const;

private:
	struct Item
	{
		Item(s2::FilterMode id, const std::string& name)
			: id(id), name(name){}

		s2::FilterMode id;
		std::string name;
	};

private:
	std::map<s2::FilterMode, int> m_map2idx;

	std::vector<Item> m_modes;

	SINGLETON_DECLARATION(FilterModes);

}; // FilterModes

}

#endif // _GLUE_FILTER_MODES_H_