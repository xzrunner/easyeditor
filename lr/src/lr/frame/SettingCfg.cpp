#include "SettingCfg.h"



#include <stdio.h>

namespace lr
{
	
SettingCfg* SettingCfg::m_instance = NULL;

SettingCfg::SettingCfg()
{
	m_map_width = ee::HALF_S_WIDTH * 2;
	m_map_height = ee::HALF_S_HEIGHT * 2;

	m_view_dx = m_view_dy = 0;
	m_view_width = m_map_width;
	m_view_height = m_map_height;

	m_terrain2d_anim = false;

	m_all_layers_visible_editable = false;

	m_special_layer_flag = false;
}

SettingCfg* SettingCfg::Instance()
{
	if (!m_instance) {
		m_instance = new SettingCfg();
	}
	return m_instance;
}

}