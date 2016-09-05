#include "FilterModes.h"

namespace glue
{

SINGLETON_DEFINITION(FilterModes);

FilterModes::FilterModes()
{
	m_modes.push_back(Item(s2::FM_NULL, "null"));

	m_modes.push_back(Item(s2::FM_EDGE_DETECTION, "edge detection"));
	m_modes.push_back(Item(s2::FM_RELIEF, "relief"));
	m_modes.push_back(Item(s2::FM_OUTLINE, "outline"));
	m_modes.push_back(Item(s2::FM_OUTER_GLOW, "outer glow"));

	m_modes.push_back(Item(s2::FM_GRAY, "gray"));
	m_modes.push_back(Item(s2::FM_BLUR, "blur"));
	m_modes.push_back(Item(s2::FM_GAUSSIAN_BLUR, "gaussian blur"));

	m_modes.push_back(Item(s2::FM_HEAT_HAZE, "heat haze"));
	m_modes.push_back(Item(s2::FM_SHOCK_WAVE, "shock wave"));
	m_modes.push_back(Item(s2::FM_SWIRL, "swirl"));
	m_modes.push_back(Item(s2::FM_BURNING_MAP, "burning map"));
}

s2::FilterMode FilterModes::Name2Mode(const std::string& name) const
{
	if (name.empty()) {
		return s2::FM_NULL;
	}

	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		const Item& item = m_modes[i];
		if (item.name == name) {
			return item.id;
		}
	}
	return s2::FM_NULL;
}

std::string FilterModes::Mode2Name(s2::FilterMode id) const
{
	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		const Item& item = m_modes[i];
		if (item.id == id) {
			return item.name;
		}
	}
	return "";
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

}