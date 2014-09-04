#include "Config.h"

#include "common/FileNameTools.h"

#include <fstream>

namespace d2d
{

Config* Config::m_instance = NULL;

const char* FILENAME = "config.json";

Config::Config()
{
	m_use_dtex = true;
}

Config* Config::Instance()
{
	if (!m_instance)
	{
		m_instance = new Config();
		if (FilenameTools::isExist(FILENAME)) {
			m_instance->LoadFromFile(FILENAME);
		}
	}
	return m_instance;
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
}

}