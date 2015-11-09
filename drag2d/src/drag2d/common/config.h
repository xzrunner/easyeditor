#ifndef _DRAG2D_CONFIG_H_
#define _DRAG2D_CONFIG_H_

#include <json/json.h>
#include <set>
#include <wx/string.h>

namespace d2d
{

struct SettingData;

class Config
{
public:
	static Config* Instance();

	bool IsUseDTex() const;
	void EnableUseDTex(bool enable) { m_use_dtex = enable; }

	bool IsRenderOpen() const { return m_use_render; }
	void EnableRender(bool enable) { m_use_render = enable; }

	const std::string& GetFontFilepath() const { return m_font_filepath; }

	const std::set<wxString>& GetResPathes() const { return m_resource_paths; }
	std::set<wxString>& GetResPathes() { return m_resource_paths; }

	void GetStrings(const std::string& key, std::vector<std::string>& val) const;

	const SettingData& GetSettings() const { return *m_settings; }
	SettingData& GetSettings() { return *m_settings; }

private:
	Config();

	void LoadFromFile(const char* filename);

	void LoadFontCfg(const Json::Value& value);

private:
	Json::Value m_value;

	bool m_use_dtex;
	bool m_use_render;

	std::string m_font_filepath;
	std::vector<std::pair<std::string, std::string> > m_fonts;

	std::set<wxString> m_resource_paths;

	SettingData* m_settings;

	static bool m_loaded;

private:
	static Config* m_instance;

}; // Config

}

#endif // _DRAG2D_CONFIG_H_