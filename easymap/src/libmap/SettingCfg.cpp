#include "SettingCfg.h"



#include <stdio.h>

namespace emap
{
	
SettingCfg* SettingCfg::m_instance = NULL;

SettingCfg::SettingCfg()
{
	m_view_width = ee::HALF_S_WIDTH * 2;
	m_view_height = ee::HALF_S_HEIGHT * 2;
}

SettingCfg* SettingCfg::Instance()
{
	if (!m_instance) {
		m_instance = new SettingCfg();
	}
	return m_instance;
}

}