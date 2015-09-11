#include "UnitCfg.h"

#include <drag2d.h>
#include <wx/stdpaths.h>

namespace lr
{

static const char* FILENAME = "unit_cfg.json";

UnitCfg* UnitCfg::m_instance = NULL;

bool UnitCfg::m_loaded = false;

UnitCfg* UnitCfg::Instance()
{
	if (!m_instance) {
		m_instance = new UnitCfg();
	}

	if (!m_loaded) {
		if (d2d::FilenameTools::IsFileExist(FILENAME)) {
			m_instance->LoadFromFile(FILENAME);
		} else {
			wxStandardPathsBase& stdp = wxStandardPaths::Get();
			wxString exe_path = stdp.GetExecutablePath();
			wxString cfg_path = d2d::FilenameTools::getFileDir(exe_path) + "\\" + FILENAME;
			if (d2d::FilenameTools::IsFileExist(cfg_path)) {
				m_instance->LoadFromFile(cfg_path);
			}
		}
	}

	return m_instance;
}

void UnitCfg::LoadFromFile(const char* filename)
{
	Json::Value val;

	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filename);
	std::locale::global(std::locale("C"));
	reader.parse(fin, val);
	fin.close();

	LoadList(val, "type", type_list);
	LoadList(val, "camp", camp_list);
	LoadList(val, "dir", dir_list);
	LoadList(val, "squad", squad_list);

	m_loaded = true;
}

void UnitCfg::LoadList(const Json::Value& val, const std::string& type, 
					   std::vector<Item>& list)
{
	list.clear();

	int idx = 0;
	Json::Value v = val[type][idx++];
	while (!v.isNull()) {
		Item item;
		item.key = v["key"].asString();
		item.title = v["title"].asString();
		list.push_back(item);

		v = val[type][idx++];
	}

}

}