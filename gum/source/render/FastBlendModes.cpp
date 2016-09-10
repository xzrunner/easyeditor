#include "FastBlendModes.h"

#include <string>

namespace gum
{

SINGLETON_DEFINITION(FastBlendModes);

FastBlendModes::FastBlendModes()
{
	m_modes.push_back(Item(s2::FBM_NULL, "null"));
	m_modes.push_back(Item(s2::FBM_ADD, "add"));
	m_modes.push_back(Item(s2::FBM_SUBTRACT, "subtract"));
}

s2::FastBlendMode FastBlendModes::Name2Mode(const std::string& name) const
{
	if (name.empty()) {
		return s2::FBM_NULL;
	}

	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		const Item& item = m_modes[i];
		if (item.name == name) {
			return item.mode;
		}
	}
	return s2::FBM_NULL;
}

std::string FastBlendModes::Mode2Name(s2::FastBlendMode mode) const
{
	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		const Item& item = m_modes[i];
		if (item.mode == mode) {
			return item.name;
		}
	}
	return "";
}

}