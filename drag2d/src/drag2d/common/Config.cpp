#include "Config.h"
#include "SettingData.h"

#include "common/FileNameTools.h"

#include <fstream>
#include <wx/stdpaths.h>

namespace d2d
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
		if (FilenameTools::IsFileExist(FILENAME)) {
			m_instance->LoadFromFile(FILENAME);
		} else {
			wxStandardPathsBase& stdp = wxStandardPaths::Get();
			wxString exe_path = stdp.GetExecutablePath();
			wxString cfg_path = FilenameTools::getFileDir(exe_path) + "\\" + FILENAME;
			if (FilenameTools::IsFileExist(cfg_path)) {
				m_instance->LoadFromFile(cfg_path);
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

	m_font_filepath = m_value["font_filepath"].asString();

	m_settings->LoadFromFile(m_value);

	m_loaded = true;
}

}