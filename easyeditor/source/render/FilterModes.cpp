#include "FilterModes.h"

namespace ee
{

SINGLETON_DEFINITION(FilterModes);

FilterModes::FilterModes()
{
	m_modes.push_back(Item(s2::FM_NULL, "无"));

	m_modes.push_back(Item(s2::FM_EDGE_DETECTION, "边界检测"));
	m_modes.push_back(Item(s2::FM_RELIEF, "浮雕"));
	m_modes.push_back(Item(s2::FM_OUTLINE, "包围"));
	m_modes.push_back(Item(s2::FM_OUTER_GLOW, "外发光"));

	m_modes.push_back(Item(s2::FM_GRAY, "灰度"));
	m_modes.push_back(Item(s2::FM_BLUR, "模糊"));
	m_modes.push_back(Item(s2::FM_GAUSSIAN_BLUR, "高斯模糊"));

	m_modes.push_back(Item(s2::FM_HEAT_HAZE, "热霾"));
	m_modes.push_back(Item(s2::FM_SHOCK_WAVE, "冲击波"));
	m_modes.push_back(Item(s2::FM_SWIRL, "漩涡"));
	m_modes.push_back(Item(s2::FM_BURNING_MAP, "烧地图"));
}

s2::FilterMode FilterModes::ID2Mode(int idx) const
{
	if (idx < 0 || idx >= static_cast<int>(m_modes.size())) {
		return s2::FM_NULL;
	} else {
		return m_modes[idx].id;
	}
}

int FilterModes::Mode2ID(s2::FilterMode mode) const
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
		names.push_back(m_modes[i].name);
	}
}

}