#ifndef _EASYEDITOR_FILTER_MODES_H_
#define _EASYEDITOR_FILTER_MODES_H_

#include <map>
#include <vector>
#include <string>

namespace ee
{

enum FilterMode
{
	FM_NORMAL = 0,

	FM_EDGE_DETECTION,
	FM_RELIEF,
	FM_OUTLINE,
	FM_BLUR,
	FM_GRAY,
	FM_HEAT_HAZE,
	FM_SHOCK_WAVE,
	FM_SWIRL,

	FM_UNKNOWN = 99,

}; // FilterMode

class FilterModes
{
public:
	FilterMode GetIDFromNameEN(const std::string& name) const;
	std::string GetNameENFromID(FilterMode id) const;

	FilterMode GetIDFromIdx(int idx) const;
	int GetIdxFromID(FilterMode mode) const;

	void GetAllNameCN(std::vector<std::string>& names) const;

	int QueryShaderIdx(FilterMode mode) const;

	static FilterModes* Instance();

private:
	FilterModes();

private:
	struct Item
	{
		Item(FilterMode id, const std::string& name_en, const std::string& name_cn)
			: id(id), name_en(name_en), name_cn(name_cn) {}

		FilterMode id;
		std::string name_en;
		std::string name_cn;
	};

private:
	std::map<FilterMode, int> m_map2idx;

	std::vector<Item> m_modes;

private:
	static FilterModes* m_instance;

}; // FilterModes

}

#endif // _EASYEDITOR_FILTER_MODES_H_