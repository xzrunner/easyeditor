#ifndef _EASYMAP_SETTING_CONFIG_H_
#define _EASYMAP_SETTING_CONFIG_H_

namespace emap
{

class SettingCfg
{
public:
	static SettingCfg* Instance();

	int m_view_width, m_view_height;

private:
	SettingCfg();

private:
	static SettingCfg* m_instance;

}; // SettingCfg

}

#endif // _EASYMAP_SETTING_CONFIG_H_