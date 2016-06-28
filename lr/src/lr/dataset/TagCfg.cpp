#include "TagCfg.h"

#include <ee/FileHelper.h>
#include <ee/Symbol.h>
#include <ee/StringHelper.h>

#include <wx/stdpaths.h>
#include <json/json.h>

#include <fstream>

namespace lr
{

static const char* FILENAME = "tag_cfg.json";

TagCfg* TagCfg::m_instance = NULL;

TagCfg* TagCfg::Instance()
{
	if (!m_instance) {
		m_instance = new TagCfg();
	}
	return m_instance;
}

TagCfg::TagCfg()
{
	if (ee::FileHelper::IsFileExist(FILENAME)) {
		LoadFromFile(FILENAME);
	} else {
		wxStandardPathsBase& stdp = wxStandardPaths::Get();
		std::string exe_path = stdp.GetExecutablePath();
		std::string cfg_path = ee::FileHelper::GetFileDir(exe_path) + "\\" + FILENAME;
		if (ee::FileHelper::IsFileExist(cfg_path)) {
			LoadFromFile(cfg_path.c_str());
		}
	}
}

std::string TagCfg::Query(const ee::Symbol* sym) const
{
	std::map<const ee::Symbol*, std::string>::iterator itr = m_cache.find(sym);
	if (itr != m_cache.end()) {
		return itr->second;
	}

	const std::string& filepath = sym->GetFilepath();
	for (int i = 0, n = m_key_val.size(); i < n; ++i) {
		const std::pair<std::string, std::string>& kv = m_key_val[i];
		if (filepath.find(kv.first) != std::string::npos) {
			m_cache.insert(std::make_pair(sym, kv.second));
			return kv.second;
		}
	}

	return "";
}

void TagCfg::LoadFromFile(const char* filename)
{
	Json::Value val;

	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filename);
	std::locale::global(std::locale("C"));
	reader.parse(fin, val);
	fin.close();

	for (int i = 0, n = val["default"].size(); i < n; ++i) {
		std::string k = val["default"][i]["symbol"].asString(),
			v = val["default"][i]["value"].asString();
		ee::StringHelper::ToLower(k);
		m_key_val.push_back(std::make_pair(k, v));
	}
}

}