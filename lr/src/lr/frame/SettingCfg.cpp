#include "SettingCfg.h"

#include <drag2d.h>

#include <stdio.h>

namespace lr
{
	
SettingCfg* SettingCfg::m_instance = NULL;

SettingCfg::SettingCfg()
{
	m_map_width = d2d::HALF_S_WIDTH * 2;
	m_map_height = d2d::HALF_S_HEIGHT * 2;

	m_view_width = m_map_width;
	m_view_height = m_map_height;

	m_terrain2d_anim = false;
}

SettingCfg* SettingCfg::Instance()
{
	if (!m_instance) {
		m_instance = new SettingCfg();
	}
	return m_instance;
}

}