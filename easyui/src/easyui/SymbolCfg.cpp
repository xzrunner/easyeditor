#include "SymbolCfg.h"

#include <wx/stdpaths.h>

namespace eui
{

static const char* FILENAME = "symbol_cfg.json";

SymbolCfg* SymbolCfg::m_instance = NULL;

void SymbolCfg::QueryType(const ee::Symbol* sym, std::string& type) const
{
	std::string filepath = sym->GetFilepath();
	filepath = ee::FileHelper::FormatFilepathAbsolute(filepath);
	for (int i = 0, n = m_widgets.size(); i < n; ++i) {
		const Widget& w = m_widgets[i];
		std::set<std::string>::const_iterator itr = w.symbols.find(filepath);
		if (itr != w.symbols.end()) {
			type = w.type;
			return;
		}
	}
}

void SymbolCfg::LoadFromFile(const char* filename)
{
	Json::Value val;

	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filename);
	std::locale::global(std::locale("C"));
	reader.parse(fin, val);
	fin.close();

	std::string dir = ee::FileHelper::GetFileDir(filename);

	int idx = 0;
	Json::Value w_val = val["widget"][idx++];
	while (!w_val.isNull()) {
		Widget w;
		w.type = w_val["type"].asString();
		const Json::Value& s_val = w_val["symbol"];
		for (int i = 0, n = s_val.size(); i < n; ++i) {
			std::string filepath = s_val[i].asString();
			filepath = ee::FileHelper::GetAbsolutePath(dir, filepath);
			filepath = ee::FileHelper::FormatFilepath(filepath);
			w.symbols.insert(filepath);
		}
		m_widgets.push_back(w);
		w_val = val["widget"][idx++];
	}
}

SymbolCfg* SymbolCfg::Instance()
{
	if (!m_instance) {
		m_instance = new SymbolCfg();

		std::string filepath = FILENAME;
		if (!ee::FileHelper::IsFileExist(filepath)) {
			wxStandardPathsBase& stdp = wxStandardPaths::Get();
			wxString exe_path = stdp.GetExecutablePath();
			filepath = ee::FileHelper::GetFileDir(exe_path) + "\\" + FILENAME;
		}
		m_instance->LoadFromFile(filepath.c_str());
	}

	return m_instance;
}

}