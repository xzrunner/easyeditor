#include "s2loader/FilterModes.h"

namespace s2loader
{

CU_SINGLETON_DEFINITION(FilterModes);

FilterModes::FilterModes()
{
	m_modes.push_back(Item(pt2::FM_NULL, "null"));

	m_modes.push_back(Item(pt2::FM_EDGE_DETECTION, "edge detection"));
	m_modes.push_back(Item(pt2::FM_RELIEF, "relief"));
	m_modes.push_back(Item(pt2::FM_OUTLINE, "outline"));
	m_modes.push_back(Item(pt2::FM_OUTER_GLOW, "outer glow"));

	m_modes.push_back(Item(pt2::FM_GRAY, "gray"));
	m_modes.push_back(Item(pt2::FM_BLUR, "blur"));
	m_modes.push_back(Item(pt2::FM_GAUSSIAN_BLUR, "gaussian blur"));

	m_modes.push_back(Item(pt2::FM_HEAT_HAZE, "heat haze"));
	m_modes.push_back(Item(pt2::FM_SHOCK_WAVE, "shock wave"));
	m_modes.push_back(Item(pt2::FM_SWIRL, "swirl"));
	m_modes.push_back(Item(pt2::FM_BURNING_MAP, "burning map"));

	m_modes.push_back(Item(pt2::FM_COL_GRADING, "color grading"));
}

pt2::FilterMode FilterModes::Name2Mode(const CU_STR& name) const
{
	if (name.empty()) {
		return pt2::FM_NULL;
	}

	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		const Item& item = m_modes[i];
		if (item.name == name) {
			return item.id;
		}
	}
	return pt2::FM_NULL;
}

CU_STR FilterModes::Mode2Name(pt2::FilterMode id) const
{
	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		const Item& item = m_modes[i];
		if (item.id == id) {
			return item.name;
		}
	}
	return "";
}

int FilterModes::QueryShaderIdx(pt2::FilterMode mode) const
{
	CU_MAP<pt2::FilterMode, int>::const_iterator itr = 
		m_map2idx.find(mode);
	if (itr == m_map2idx.end()) {
		return 0;
	} else {
		return itr->second;
	}
}

}