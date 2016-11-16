#ifndef _LR_SETTING_CONFIG_H_
#define _LR_SETTING_CONFIG_H_

#include <string>

namespace lr
{

class SettingCfg
{
public:
	static SettingCfg* Instance();

	int m_map_width, m_map_height;

	int m_view_dx, m_view_dy,
		m_view_width, m_view_height;

	bool m_terrain2d_anim;

	bool m_all_layers_visible_editable;

	bool m_special_layer_flag;

	std::string m_post_effect_file;

private:
	SettingCfg();

private:
	static SettingCfg* m_instance;

}; // SettingCfg

}

#endif // _LR_SETTING_CONFIG_H_