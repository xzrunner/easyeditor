#include "SymbolsCfg.h"
#include "LibraryPage.h"
#include "StagePanel.h"

#include <ee/std_functor.h>
#include <ee/Exception.h>
#include <ee/ExceptionDlg.h>
#include <ee/LibraryPanel.h>
#include <ee/LibraryList.h>
#include <ee/SymbolMgr.h>
#include <ee/Symbol.h>
#include <ee/SymbolFile.h>
#include <ee/SymbolType.h>

#include <algorithm>
#include <fstream>

namespace eui
{
namespace window
{

SymbolsCfg::SymbolsCfg(StagePanel* stage, ee::LibraryPanel* library)
	: m_stage(stage)
	, m_library(library)
{
}

SymbolsCfg::~SymbolsCfg()
{
	for_each(m_syms.begin(), m_syms.end(), ee::DeletePointerFunctor<Symbol>());
}

void SymbolsCfg::LoadConfig()
{
	const char* config_filepath = "symbols_cfg.json";

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(config_filepath);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	try {
		InitLibrarySymbols(value);
	} catch (ee::Exception& e) {
		ee::ExceptionDlg dlg(m_stage, e);
		dlg.ShowModal();	
	}
}

void SymbolsCfg::InitLibrarySymbols(const Json::Value& value)
{
	InitLibrarySymbols(value["window"], "窗口", ee::SymbolFile::Instance()->Tag(ee::SYM_UIWND));
	InitLibrarySymbols(value["button"], "按钮");
	InitLibrarySymbols(value["list"], "列表");
	InitLibrarySymbols(value["textbox"], "文本框");
	InitLibrarySymbols(value["panel"], "面板");
	InitLibrarySymbols(value["other"], "其他");
}

void SymbolsCfg::InitLibrarySymbols(const Json::Value& value, const std::string& name, const std::string& filter)
{
	std::vector<Symbol*> syms;

	InitLibrarySymbols(value, syms);

	wxWindow* nb = m_library->GetNotebook();
	LibraryPage* page = new LibraryPage(nb, name.c_str(), filter);
	m_library->AddPage(page);

	ResetLibraryList(page, syms);

	copy(syms.begin(), syms.end(), back_inserter(m_syms));
}

void SymbolsCfg::InitLibrarySymbols(const Json::Value& value, std::vector<Symbol*>& syms)
{
	int i = 0;
	Json::Value symbol_val = value[i++];
	while (!symbol_val.isNull()) {
		Symbol* s = new Symbol;
		s->filepath = symbol_val["filepath"].asString();
		syms.push_back(s);

		symbol_val = value[i++];
	}
}

void SymbolsCfg::ResetLibraryList(LibraryPage* library, const std::vector<Symbol*>& syms)
{
	library->GetList()->Clear();

	for (int i = 0, n = syms.size(); i < n; ++i)
	{
		const Symbol* src = syms[i];

		ee::Symbol* dst = ee::SymbolMgr::Instance()->FetchSymbol(src->filepath);
		dst->RefreshThumbnail(dst->GetFilepath());
		library->GetList()->Insert(dst);
		dst->RemoveReference();
	}
}

}
}