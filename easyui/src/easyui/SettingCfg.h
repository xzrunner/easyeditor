#ifndef _EASYUI_SETTING_CONFIG_H_
#define _EASYUI_SETTING_CONFIG_H_

namespace eui
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

#endif // _EASYUI_SETTING_CONFIG_H_