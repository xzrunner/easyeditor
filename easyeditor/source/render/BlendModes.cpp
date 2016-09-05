#include "BlendModes.h"

#include <string>

namespace ee
{

SINGLETON_DEFINITION(BlendModes);

BlendModes::BlendModes()
{
	// normal
	m_modes.push_back(Item(s2::BM_NULL, "无"));
//	m_modes.push_back(Item(s2::BM_DISSOLVE, "溶解"));

	// darken modes
	m_modes.push_back(Item(s2::BM_DARKEN, "变暗"));
	m_modes.push_back(Item(s2::BM_MULTIPLY, "正片叠底"));
	m_modes.push_back(Item(s2::BM_COLOR_BURN, "颜色加深"));
	m_modes.push_back(Item(s2::BM_LINEAR_BURN, "线性颜色加深"));
//	m_modes.push_back(Item(s2::BM_DARKER_COLOR, "?"));

	// lighten modes
	m_modes.push_back(Item(s2::BM_LIGHTEN, "变亮"));
	m_modes.push_back(Item(s2::BM_SCREEN, "屏幕"));
	m_modes.push_back(Item(s2::BM_COLOR_DODGE, "颜色减淡"));
	m_modes.push_back(Item(s2::BM_LINEAR_DODGE, "线性颜色减淡"));
//	m_modes.push_back(Item(s2::BM_LIGHTER_COLOR, "?"));

	// saturation modes
	m_modes.push_back(Item(s2::BM_OVERLAY, "叠加"));
	m_modes.push_back(Item(s2::BM_SOFT_LIGHT, "柔光"));
	m_modes.push_back(Item(s2::BM_HARD_LIGHT, "强光"));
	m_modes.push_back(Item(s2::BM_VIVID_LIGHT, "亮光"));
	m_modes.push_back(Item(s2::BM_LINEAR_LIGHT, "线性光"));
	m_modes.push_back(Item(s2::BM_PIN_LIGHT, "固定光"));
	m_modes.push_back(Item(s2::BM_HARD_MIX, "实色混合"));

	// substraction modes
	m_modes.push_back(Item(s2::BM_DIFFERENCE, "差异"));
	m_modes.push_back(Item(s2::BM_EXCLUSION, "排除"));

	// color modes 
// 	m_modes.push_back(Item(s2::BM_HUE, "色调"));
// 	m_modes.push_back(Item(s2::BM_SATURATION, "饱和度"));
// 	m_modes.push_back(Item(s2::BM_COLOR, "着色"));
// 	m_modes.push_back(Item(s2::BM_LUMINOSITY, "明度"));
}

s2::BlendMode BlendModes::ID2Mode(int id) const
{
	if (id < 0 || id >= static_cast<int>(m_modes.size())) {
		return s2::BM_NULL;
	} else {
		return m_modes[id].mode;
	}
}

int BlendModes::Mode2ID(s2::BlendMode mode) const
{
	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		if (m_modes[i].mode == mode) {
			return i;
		}
	}
	return 0;
}

void BlendModes::GetAllNameCN(std::vector<std::string>& names) const
{
	names.clear();
	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		names.push_back(m_modes[i].name);
	}
}

}