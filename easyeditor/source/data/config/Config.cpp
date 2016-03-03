#include "Config.h"
#include "SettingData.h"
#include "FileHelper.h"

#include <wx/stdpaths.h>

#include <fstream>

namespace ee
{

Config* Config::m_instance = NULL;

bool Config::m_loaded = false;

static const char* FILENAME = "config.json";

Config::Config()
{
	m_use_dtex = true;
	//m_use_dtex = false;

	m_use_render = true;

	m_settings = new SettingData;
}

Config* Config::Instance()
{
	if (!m_instance) {
		m_instance = new Config();
	}

	if (!m_loaded) {
		if (FileHelper::IsFileExist(FILENAME)) {
			m_instance->LoadFromFile(FILENAME);
		} else {
			wxStandardPathsBase& stdp = wxStandardPaths::Get();
			std::string exe_path = stdp.GetExecutablePath();
			std::string cfg_path = FileHelper::GetFileDir(exe_path) + "\\" + FILENAME;
			if (FileHelper::IsFileExist(cfg_path)) {
				m_instance->LoadFromFile(cfg_path.c_str());
			}
		}
	}

	return m_instance;
}

bool Config::IsUseDTex() const 
{
	return m_use_dtex; 
}

void Config::GetStrings(const std::string& key, 
						std::vector<std::string>& val) const
{
	Json::Value str_list = m_value[key];
	if (!str_list.isNull())
	{
		int i = 0;
		Json::Value str = str_list[i++];
		while (!str.isNull()) {
			val.push_back(str.asString());
			str = str_list[i++];
		}
	}
}

void Config::LoadFromFile(const char* filename)
{
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filename);
	std::locale::global(std::locale("C"));
	reader.parse(fin, m_value);
	fin.close();

	m_use_dtex = m_value["use_dtex"].asBool();

	LoadFontCfg(m_value["font"]);

	m_settings->LoadFromFile(m_value);

	m_loaded = true;
}

void Config::LoadFontCfg(const Json::Value& value)
{
	int idx = 0;
	Json::Value font_val = value[idx++];
	while (!font_val.isNull()) {
		std::string name = font_val["name"].asString();
		std::string filepath = font_val["filepath"].asString();
		m_fonts.push_back(std::make_pair(name, filepath));
		font_val = value[idx++];
	}
}

}