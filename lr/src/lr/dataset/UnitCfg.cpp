#include "UnitCfg.h"
#include "UnitWidget.h"

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

	int idx = 0;
	Json::Value w_val = val["widget"][idx++];
	while (!w_val.isNull()) {
		std::string type = w_val["type"].asString();
		UnitWidget* widget = NULL;
		if (type == "text") {
			widget = new UnitTextWidget;
		} else if (type == "choice") {
			widget = new UnitChoiceWidget;
		} else if (type == "check box") {
			widget = new UnitCheckBoxWidget;
		} else {
			throw d2d::Exception("UnitCfg::LoadFromFile unknown type %s", type.c_str());
		}
		widget->LoadFromFile(w_val);
		widgets.push_back(widget);
		w_val = val["widget"][idx++];
	}

	m_loaded = true;
}

}