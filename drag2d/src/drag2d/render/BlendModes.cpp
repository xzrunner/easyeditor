#include "BlendModes.h"

#include <string>

namespace d2d
{

BlendModes* BlendModes::m_instance = NULL;

BlendModes::BlendModes()
{
	// normal
	m_modes.push_back(Item(BM_NORMAL, "normal", "正常"));
//	m_modes.push_back(Item(BM_DISSOLVE, "dissolve", "溶解"));

	// darken modes
	m_modes.push_back(Item(BM_DARKEN, "darken", "变暗"));
	m_modes.push_back(Item(BM_MULTIPLY, "multiply", "正片叠底"));
	m_modes.push_back(Item(BM_COLOR_BURN, "color burn", "颜色加深"));
	m_modes.push_back(Item(BM_LINEAR_BURN, "linear burn", "线性颜色加深"));
//	m_modes.push_back(Item(BM_DARKER_COLOR, "darker color", "?"));

	// lighten modes
	m_modes.push_back(Item(BM_LIGHTEN, "lighten", "变亮"));
	m_modes.push_back(Item(BM_SCREEN, "screen", "屏幕"));
	m_modes.push_back(Item(BM_COLOR_DODGE, "color dodge", "颜色减淡"));
	m_modes.push_back(Item(BM_LINEAR_DODGE, "linear dodge", "线性颜色减淡"));
//	m_modes.push_back(Item(BM_LIGHTER_COLOR, "lighter color", "?"));

	// saturation modes
	m_modes.push_back(Item(BM_OVERLAY, "overlay", "叠加"));
	m_modes.push_back(Item(BM_SOFT_LIGHT, "soft light", "柔光"));
	m_modes.push_back(Item(BM_HARD_LIGHT, "hard light", "强光"));
	m_modes.push_back(Item(BM_VIVID_LIGHT, "vivid light", "艳光"));
	m_modes.push_back(Item(BM_LINEAR_LIGHT, "linear light", "线性光"));
	m_modes.push_back(Item(BM_PIN_LIGHT, "pin light", "固定光"));
	m_modes.push_back(Item(BM_HARD_MIX, "hard mix", "实色混合"));

	// substraction modes
	m_modes.push_back(Item(BM_DIFFERENCE, "difference", "差异"));
	m_modes.push_back(Item(BM_EXCLUSION, "exclusion", "排除"));

	// color modes 
// 	m_modes.push_back(Item(BM_HUE, "hue", "色调"));
// 	m_modes.push_back(Item(BM_SATURATION, "saturation", "饱和度"));
// 	m_modes.push_back(Item(BM_COLOR, "color", "着色"));
// 	m_modes.push_back(Item(BM_LUMINOSITY, "luminosity", "明度"));
}

BlendMode BlendModes::GetIDFromNameEN(const std::string& name) const
{
	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		const Item& item = m_modes[i];
		if (item.name_cn == name) {
			return item.id;
		}
	}
	return BM_UNKNOWN;
}

BlendMode BlendModes::GetIDFromIdx(int idx) const
{
	if (idx < 0 || idx >= m_modes.size()) {
		return BM_UNKNOWN;
	} else {
		return m_modes[idx].id;
	}
}

int BlendModes::GetIdxFromID(BlendMode mode) const
{
	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		if (m_modes[i].id == mode) {
			return i;
		}
	}
	return 0;
}

void BlendModes::GetAllNameCN(wxArrayString& names) const
{
	names.clear();
	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		names.push_back(m_modes[i].name_cn);
	}
}

BlendModes* BlendModes::Instance()
{
	if (!m_instance) {
		m_instance = new BlendModes();
	}
	return m_instance;
}

}