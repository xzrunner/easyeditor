#include "BlendModes.h"

#include <string>

namespace glue
{

SINGLETON_DEFINITION(BlendModes);

BlendModes::BlendModes()
{
	// normal
	m_modes.push_back(Item(s2::BM_NULL, "null"));
//	m_modes.push_back(Item(s2::BM_DISSOLVE, "dissolve"));

	// darken modes
	m_modes.push_back(Item(s2::BM_DARKEN, "darken"));
	m_modes.push_back(Item(s2::BM_MULTIPLY, "multiply"));
	m_modes.push_back(Item(s2::BM_COLOR_BURN, "color burn"));
	m_modes.push_back(Item(s2::BM_LINEAR_BURN, "linear burn"));
//	m_modes.push_back(Item(s2::BM_DARKER_COLOR, "darker color"));

	// lighten modes
	m_modes.push_back(Item(s2::BM_LIGHTEN, "lighten"));
	m_modes.push_back(Item(s2::BM_SCREEN, "screen"));
	m_modes.push_back(Item(s2::BM_COLOR_DODGE, "color dodge"));
	m_modes.push_back(Item(s2::BM_LINEAR_DODGE, "linear dodge"));
//	m_modes.push_back(Item(s2::BM_LIGHTER_COLOR, "lighter color"));

	// saturation modes
	m_modes.push_back(Item(s2::BM_OVERLAY, "overlay"));
	m_modes.push_back(Item(s2::BM_SOFT_LIGHT, "soft light"));
	m_modes.push_back(Item(s2::BM_HARD_LIGHT, "hard light"));
	m_modes.push_back(Item(s2::BM_VIVID_LIGHT, "vivid light"));
	m_modes.push_back(Item(s2::BM_LINEAR_LIGHT, "linear light"));
	m_modes.push_back(Item(s2::BM_PIN_LIGHT, "pin light"));
	m_modes.push_back(Item(s2::BM_HARD_MIX, "hard mix"));

	// substraction modes
	m_modes.push_back(Item(s2::BM_DIFFERENCE, "difference"));
	m_modes.push_back(Item(s2::BM_EXCLUSION, "exclusion"));

	// color modes 
// 	m_modes.push_back(Item(s2::BM_HUE, "hue"));
// 	m_modes.push_back(Item(s2::BM_SATURATION, "saturation"));
// 	m_modes.push_back(Item(s2::BM_COLOR, "color"));
// 	m_modes.push_back(Item(s2::BM_LUMINOSITY, "luminosity"));
}

s2::BlendMode BlendModes::Name2Mode(const std::string& name) const
{
	if (name.empty()) {
		return s2::BM_NULL;
	}

	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		const Item& item = m_modes[i];
		if (item.name == name) {
			return item.mode;
		}
	}
	return s2::BM_NULL;
}

std::string BlendModes::ModeToName(s2::BlendMode mode) const
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