#ifndef _EASYEDITOR_CONFIG_H_
#define _EASYEDITOR_CONFIG_H_

#include <cu/cu_stl.h>

#include <json/json.h>

#include <string>
#include <set>

namespace ee
{

class SettingData;

class Config
{
public:
	static Config* Instance();

	bool IsUseDTex() const;
	void EnableUseDTex(bool enable) { m_use_dtex = enable; }

	bool IsRenderOpen() const { return m_use_render; }
	void EnableRender(bool enable) { m_use_render = enable; }

	bool IsDebugDrawOpen() const { return m_debug_draw; }

	const std::set<std::string>& GetResPathes() const { return m_resource_paths; }
	std::set<std::string>& GetResPathes() { return m_resource_paths; }

	void GetStrings(const std::string& key, std::vector<std::string>& val) const;

	const SettingData& GetSettings() const { return *m_settings; }
	SettingData& GetSettings() { return *m_settings; }

	auto& GetFonts() const { return m_fonts; }
	auto& GetUserFonts() const { return m_user_fonts; }

private:
	Config();

	void LoadFromFile(const char* filename);

	void LoadFontCfg(const Json::Value& value);
	void LoadUserFontCfg(const Json::Value& value);

private:
	Json::Value m_value;

	bool m_use_dtex;
	bool m_use_render;

	bool m_debug_draw;

	CU_VEC<std::pair<CU_STR, CU_STR>> m_fonts;
	CU_VEC<std::pair<CU_STR, CU_STR>> m_user_fonts;

	std::set<std::string> m_resource_paths;

	SettingData* m_settings;

	static bool m_loaded;

private:
	static Config* m_instance;

}; // Config

}

#endif // _EASYEDITOR_CONFIG_H_