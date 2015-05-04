#include "SymbolsCfg.h"

#include "view/LibraryPage.h"
#include "view/StagePanel.h"

namespace lr
{

static const char* SYMBOL_CFG_PATH = "symbols_cfg.json";

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
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(SYMBOL_CFG_PATH);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	try {
		InitLibrarySymbols(value);
		m_library->setCanvas(m_stage->getCanvas());
	} catch (d2d::Exception& e) {
		d2d::ExceptionDlg dlg(m_stage, e);
		dlg.ShowModal();	
	}
}

void SymbolsCfg::StoreLibraryPage(const std::string& key, d2d::ILibraryPage* page)
{
	if (!page->getList()->getSymbol(0)) {
		return;
	}

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(SYMBOL_CFG_PATH);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

 	Json::Value& page_val = value[key];

	std::set<std::string> filepathes;
	int idx = 0;
	Json::Value spr_val = page_val[idx++];
	while (!spr_val.isNull()) {
		filepathes.insert(spr_val["filepath"].asString());
		spr_val = page_val[idx++];
	}

	wxFileName filename(SYMBOL_CFG_PATH);
	filename.Normalize();
	std::string dir = d2d::FilenameTools::getFileDir(filename.GetFullPath().Lower());

	idx = 0;
	while (d2d::ISymbol* sym = page->getList()->getSymbol(idx++))
	{
		std::string filepath = d2d::FilenameTools::getRelativePath(dir, sym->getFilepath());
		filepathes.insert(filepath);
	}

	std::set<std::string>::iterator itr = filepathes.begin();
	for (int i = 0; itr != filepathes.end(); ++itr, ++i) {
		page_val[i]["filepath"] = *itr;
	}

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(SYMBOL_CFG_PATH);
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void SymbolsCfg::InitLibrarySymbols(const Json::Value& value)
{
	InitLibrarySymbols(value["terrain"], "地形", "terrain");
	InitLibrarySymbols(value["unit"], "单位", "unit");
	InitLibrarySymbols(value["decoration"], "装饰", "decoration");
	InitLibrarySymbols(value["collision region"], "碰撞区", "collision region");
	InitLibrarySymbols(value["point"], "点", "point");
	InitLibrarySymbols(value["path"], "路径", "path");
	InitLibrarySymbols(value["region"], "区域", "region");
}

void SymbolsCfg::InitLibrarySymbols(const Json::Value& value, const std::string& name,
									const std::string& key)
{
	std::vector<Symbol*> symbols;

	InitLibrarySymbols(value, symbols);

	wxWindow* nb = m_library->getNotebook();
	LibraryPage* page = new LibraryPage(nb, name.c_str(), key, m_stage);
	m_library->addPage(page);

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