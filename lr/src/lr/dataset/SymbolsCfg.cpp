#include "SymbolsCfg.h"

#include "view/LibraryPage.h"
#include "view/StagePanel.h"

namespace lr
{

SymbolsCfg::SymbolsCfg(StagePanel* stage, d2d::LibraryPanel* library)
	: m_stage(stage)
	, m_library(library)
{
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
	} catch (d2d::Exception& e) {
		d2d::ExceptionDlg dlg(m_stage, e);
		dlg.ShowModal();	
	}
}

void SymbolsCfg::InitLibrarySymbols(const Json::Value& value)
{
	InitLibrarySymbols(value["terrain"], m_symbols.terrain);
	InitLibrarySymbols(value["unit"], m_symbols.unit);
	InitLibrarySymbols(value["decoration"], m_symbols.decoration);

	wxWindow* nb = m_library->getNotebook();
	LibraryPage* defenses = new LibraryPage(nb, "Terrain");
	m_library->addPage(defenses);
	LibraryPage* resources = new LibraryPage(nb, "Unit");
	m_library->addPage(resources);
	LibraryPage* army = new LibraryPage(nb, "Decoration");
	m_library->addPage(army);	

	ResetLibraryList(defenses, m_symbols.terrain);
	ResetLibraryList(resources, m_symbols.unit);
	ResetLibraryList(army, m_symbols.decoration);
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
	library->getList()->clear();

	for (int i = 0, n = symbols.size(); i < n; ++i)
	{
		const Symbol* src = symbols[i];

		d2d::ISymbol* dst = d2d::SymbolMgr::Instance()->fetchSymbol(src->filepath);
		library->getList()->insert(dst);
		dst->Release();
	}
}

}