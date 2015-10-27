#include "SymbolsCfg.h"
#include "LibraryPage.h"
#include "StagePanel.h"

namespace eui
{
namespace overall
{

SymbolsCfg::SymbolsCfg(StagePanel* stage, d2d::LibraryPanel* library)
	: m_stage(stage)
	, m_library(library)
{
}

SymbolsCfg::~SymbolsCfg()
{
	for_each(m_symbols.begin(), m_symbols.end(), DeletePointerFunctor<Symbol>());
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
		m_stage->ResetCanvas();
	} catch (d2d::Exception& e) {
		d2d::ExceptionDlg dlg(m_stage, e);
		dlg.ShowModal();	
	}
}

void SymbolsCfg::InitLibrarySymbols(const Json::Value& value)
{
	InitLibrarySymbols(value["button"], "按钮");
	InitLibrarySymbols(value["list"], "列表");
	InitLibrarySymbols(value["textbox"], "文本框");
	InitLibrarySymbols(value["panel"], "面板");
	InitLibrarySymbols(value["other"], "其他");
}

void SymbolsCfg::InitLibrarySymbols(const Json::Value& value, const std::string& name)
{
	std::vector<Symbol*> symbols;

	InitLibrarySymbols(value, symbols);

	wxWindow* nb = m_library->GetNotebook();
	LibraryPage* page = new LibraryPage(nb, name.c_str());
	m_library->AddPage(page);

	ResetLibraryList(page, symbols);

	copy(symbols.begin(), symbols.end(), back_inserter(m_symbols));
}

void SymbolsCfg::InitLibrarySymbols(const Json::Value& value, std::vector<Symbol*>& symbols)
{
	int i = 0;
	Json::Value symbol_val = value[i++];
	while (!symbol_val.isNull()) {
		Symbol* s = new Symbol;
		s->filepath = symbol_val["filepath"].asString();
		symbols.push_back(s);

		symbol_val = value[i++];
	}
}

void SymbolsCfg::ResetLibraryList(LibraryPage* library, const std::vector<Symbol*>& symbols)
{
	library->GetList()->Clear();

	for (int i = 0, n = symbols.size(); i < n; ++i)
	{
		const Symbol* src = symbols[i];

		d2d::ISymbol* dst = d2d::SymbolMgr::Instance()->FetchSymbol(src->filepath);
		dst->RefreshThumbnail(dst->GetFilepath());
		library->GetList()->Insert(dst);
		dst->Release();
	}
}

}
}