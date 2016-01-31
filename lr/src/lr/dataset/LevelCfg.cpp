#include "LevelCfg.h"
#include "WidgetFactory.h"



#include <wx/stdpaths.h>

namespace lr
{

static const char* FILENAME = "level_cfg.json";

LevelCfg* LevelCfg::m_instance = NULL;

LevelCfg* LevelCfg::Instance()
{
	if (!m_instance) {
		m_instance = new LevelCfg();
	}
	return m_instance;
}

LevelCfg::LevelCfg()
{
	if (ee::FileHelper::IsFileExist(FILENAME)) {
		LoadFromFile(FILENAME);
	} else {
		wxStandardPathsBase& stdp = wxStandardPaths::Get();
		wxString exe_path = stdp.GetExecutablePath();
		wxString cfg_path = ee::FileHelper::GetFileDir(exe_path) + "\\" + FILENAME;
		if (ee::FileHelper::IsFileExist(cfg_path)) {
			LoadFromFile(cfg_path);
		}
	}
}

bool LevelCfg::QueryLayout(const std::string& type, int& col, 
						   std::vector<DynamicWidget*>& widgets)
{
	std::map<std::string, Dialog*>::iterator itr = m_dialogs.find(type);
	if (itr == m_dialogs.end()) {
		return false;
	}

	Dialog* d = itr->second;
	col = d->col;
	widgets = d->widgets;

	return true;
}

void LevelCfg::LoadFromFile(const char* filename)
{
	Json::Value val;

	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filename);
	std::locale::global(std::locale("C"));
	reader.parse(fin, val);
	fin.close();

	int idx = 0;
	Json::Value d_val = val["dialog"][idx++];
	while (!d_val.isNull()) {
		InsertDialog(d_val);
		d_val = val["dialog"][idx++];
	}
}

void LevelCfg::InsertDialog(const Json::Value& val)
{
	Dialog* dlg = new Dialog;

	dlg->type = val["type"].asString();
	dlg->col = val["layout"]["col"].asInt();

	int idx = 0;
	Json::Value w_val = val["widget"][idx++];
	while (!w_val.isNull()) {
		dlg->widgets.push_back(WidgetFactory::Instance()->Create(w_val));
		w_val = val["widget"][idx++];
	}

	m_dialogs.insert(std::make_pair(dlg->type, dlg));
}

}