#include "FastBlendModes.h"

#include <string>

namespace ee
{

FastBlendModes* FastBlendModes::m_instance = NULL;

FastBlendModes::FastBlendModes()
{
	m_modes.push_back(Item(s2::FBM_NULL, "null", "无"));
	m_modes.push_back(Item(s2::FBM_ADD, "add", "相加"));
	m_modes.push_back(Item(s2::FBM_SUBTRACT, "subtract", "相减"));
}

s2::FastBlendMode FastBlendModes::GetModeFromNameEN(const std::string& name) const
{
	if (name.empty()) {
		return s2::FBM_NULL;
	}

	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		const Item& item = m_modes[i];
		if (item.name_en == name) {
			return item.mode;
		}
	}
	return s2::FBM_NULL;
}

std::string FastBlendModes::GetNameENFromMode(s2::FastBlendMode mode) const
{
	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		const Item& item = m_modes[i];
		if (item.mode == mode) {
			return item.name_en;
		}
	}
	return "";
}

s2::FastBlendMode FastBlendModes::GetIDFromIdx(int idx) const
{
	if (idx < 0 || idx >= static_cast<int>(m_modes.size())) {
		return s2::FBM_NULL;
	} else {
		return m_modes[idx].mode;
	}
}

int FastBlendModes::GetIdxFromID(s2::FastBlendMode mode) const
{
	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		if (m_modes[i].mode == mode) {
			return i;
		}
	}
	return 0;
}

void FastBlendModes::GetAllNameCN(std::vector<std::string>& names) const
{
	names.clear();
	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		names.push_back(m_modes[i].name_cn);
	}
}

FastBlendModes* FastBlendModes::Instance()
{
	if (!m_instance) {
		m_instance = new FastBlendModes();
	}
	return m_instance;
}

}