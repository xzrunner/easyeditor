#include "FilterModes.h"

namespace ee
{

FilterModes* FilterModes::m_instance = NULL;

FilterModes::FilterModes()
{
	m_modes.push_back(Item(s2::FM_NULL, "null", "无"));

	m_modes.push_back(Item(s2::FM_EDGE_DETECTION, "edge detection", "边界检测"));
	m_modes.push_back(Item(s2::FM_RELIEF, "relief", "浮雕"));
	m_modes.push_back(Item(s2::FM_OUTLINE, "outline", "包围"));

	m_modes.push_back(Item(s2::FM_GRAY, "gray", "灰度"));
	m_modes.push_back(Item(s2::FM_BLUR, "blur", "模糊"));
	m_modes.push_back(Item(s2::FM_GAUSSIAN_BLUR, "gaussian blur", "高斯模糊"));

	m_modes.push_back(Item(s2::FM_HEAT_HAZE, "heat haze", "热霾"));
	m_modes.push_back(Item(s2::FM_SHOCK_WAVE, "shock wave", "冲击波"));
	m_modes.push_back(Item(s2::FM_SWIRL, "swirl", "漩涡"));
	m_modes.push_back(Item(s2::FM_BURNING_MAP, "burning map", "烧地图"));
}

s2::FilterMode FilterModes::GetModeFromNameEN(const std::string& name) const
{
	if (name.empty()) {
		return s2::FM_NULL;
	}

	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		const Item& item = m_modes[i];
		if (item.name_en == name) {
			return item.id;
		}
	}
	return s2::FM_NULL;
}

std::string FilterModes::GetNameENFromMode(s2::FilterMode id) const
{
	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		const Item& item = m_modes[i];
		if (item.id == id) {
			return item.name_en;
		}
	}
	return "";
}

s2::FilterMode FilterModes::GetModeFromIdx(int idx) const
{
	if (idx < 0 || idx >= static_cast<int>(m_modes.size())) {
		return s2::FM_NULL;
	} else {
		return m_modes[idx].id;
	}
}

int FilterModes::GetIdxFromMode(s2::FilterMode mode) const
{
	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		if (m_modes[i].id == mode) {
			return i;
		}
	}
	return 0;
}

void FilterModes::GetAllNameCN(std::vector<std::string>& names) const
{
	names.clear();
	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		names.push_back(m_modes[i].name_cn);
	}
}

int FilterModes::QueryShaderIdx(s2::FilterMode mode) const
{
	std::map<s2::FilterMode, int>::const_iterator itr = 
		m_map2idx.find(mode);
	if (itr == m_map2idx.end()) {
		return 0;
	} else {
		return itr->second;
	}
}

FilterModes* FilterModes::Instance()
{
	if (!m_instance) {
		m_instance = new FilterModes();
	}
	return m_instance;
}

}