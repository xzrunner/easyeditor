#ifndef _EASYEDITOR_FILTER_MODES_H_
#define _EASYEDITOR_FILTER_MODES_H_

#include <sprite2/FilterMode.h>

#include <map>
#include <vector>
#include <string>

namespace ee
{

class FilterModes
{
public:
	s2::FilterMode GetModeFromNameEN(const std::string& name) const;
	std::string GetNameENFromMode(s2::FilterMode id) const;

	s2::FilterMode GetModeFromIdx(int idx) const;
	int GetIdxFromMode(s2::FilterMode mode) const;

	void GetAllNameCN(std::vector<std::string>& names) const;

	int QueryShaderIdx(s2::FilterMode mode) const;

	static FilterModes* Instance();

private:
	FilterModes();

private:
	struct Item
	{
		Item(s2::FilterMode id, const std::string& name_en, const std::string& name_cn)
			: id(id), name_en(name_en), name_cn(name_cn) {}

		s2::FilterMode id;
		std::string name_en;
		std::string name_cn;
	};

private:
	std::map<s2::FilterMode, int> m_map2idx;

	std::vector<Item> m_modes;

private:
	static FilterModes* m_instance;

}; // FilterModes

}

#endif // _EASYEDITOR_FILTER_MODES_H_