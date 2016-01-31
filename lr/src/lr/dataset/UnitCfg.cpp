#include "UnitCfg.h"
#include "WidgetFactory.h"


#include <wx/stdpaths.h>

namespace lr
{

static const char* FILENAME = "unit_cfg.json";

UnitCfg* UnitCfg::m_instance = NULL;

UnitCfg* UnitCfg::Instance()
{
	if (!m_instance) {
		m_instance = new UnitCfg();
	}
	return m_instance;
}

UnitCfg::UnitCfg()
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

void UnitCfg::LoadFromFile(const char* filename)
{
	Json::Value val;

	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filename);
	std::locale::global(std::locale("C"));
	reader.parse(fin, val);
	fin.close();

	col = val["layout"]["col"].asInt();

	int idx = 0;
	Json::Value w_val = val["widget"][idx++];
	while (!w_val.isNull()) {
		widgets.push_back(WidgetFactory::Instance()->Create(w_val));
		w_val = val["widget"][idx++];
	}
}

}