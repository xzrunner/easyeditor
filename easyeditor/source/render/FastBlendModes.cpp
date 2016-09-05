#include "FastBlendModes.h"

#include <string>

namespace ee
{

SINGLETON_DEFINITION(FastBlendModes);

FastBlendModes::FastBlendModes()
{
	m_modes.push_back(Item(s2::FBM_NULL, "无"));
	m_modes.push_back(Item(s2::FBM_ADD, "相加"));
	m_modes.push_back(Item(s2::FBM_SUBTRACT, "相减"));
}

s2::FastBlendMode FastBlendModes::ID2Mode(int id) const
{
	if (id < 0 || id >= static_cast<int>(m_modes.size())) {
		return s2::FBM_NULL;
	} else {
		return m_modes[id].mode;
	}
}

int FastBlendModes::Mode2ID(s2::FastBlendMode mode) const
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
		names.push_back(m_modes[i].name);
	}
}

}