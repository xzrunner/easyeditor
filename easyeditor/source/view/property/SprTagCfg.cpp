#include "SprTagCfg.h"
#include "WidgetFactory.h"
#include "FileHelper.h"

#include <wx/stdpaths.h>

#include <fstream>

namespace ee
{

SINGLETON_DEFINITION(SprTagCfg);

static const char* FILENAME = "tag_cfg.json";

SprTagCfg::SprTagCfg()
{
	if (FileHelper::IsFileExist(FILENAME)) {
		LoadFromFile(FILENAME);
	} else {
		wxStandardPathsBase& stdp = wxStandardPaths::Get();
		std::string exe_path = stdp.GetExecutablePath();
		std::string cfg_path = FileHelper::GetFileDir(exe_path) + "\\" + FILENAME;
		if (FileHelper::IsFileExist(cfg_path)) {
			LoadFromFile(cfg_path.c_str());
		}
	}
}

void SprTagCfg::LoadFromFile(const char* filename)
{
	Json::Value val;

	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filename);
	std::locale::global(std::locale("C"));
	reader.parse(fin, val);
	fin.close();

	for (int i = 0, n = val["widget"].size(); i < n; ++i) {
		m_widgets.push_back(WidgetFactory::Instance()->Create(val["widget"][i]));
	}
}

}