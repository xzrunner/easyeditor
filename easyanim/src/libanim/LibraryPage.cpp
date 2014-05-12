#include "LibraryPage.h"
#include "Symbol.h"
#include "config.h"

#include <easycomplex.h>
#include <easycoco.h>

namespace anim
{

LibraryPage::LibraryPage(wxWindow* parent)
	: d2d::ILibraryPage(parent, wxT("Anim"))
{
	initLayout();
	m_list->setFileter(eanim::FILE_TAG);
}

bool LibraryPage::isHandleSymbol(d2d::ISymbol* symbol) const
{
	return dynamic_cast<Symbol*>(symbol) != NULL;
}

void LibraryPage::onAddPress(wxCommandEvent& event)
{
	wxString filter = d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_anim);
	filter = wxT("*_") + filter + wxT(".json");
	filter += wxT("; *.lua");
	wxFileDialog dlg(this, wxT("导入anim文件"), wxEmptyString, 
		wxEmptyString, filter, wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			const wxString filename = filenames[i];
			std::string type = filename.substr(filename.find_last_of(".") + 1);
			StringTools::toLower(type);
			if (type == "json") {
				loadFromJsonFile(filename);
				m_canvas->resetInitState();
			} else if (type == "lua") {
				loadFromLuaFile(filename);
			}
		}
	}
}

void LibraryPage::loadFromJsonFile(const wxString& filename)
{
	d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filename);
	symbol->refresh();
	m_list->insert(symbol);
	symbol->release();
}

void LibraryPage::loadFromLuaFile(const wxString& filename)
{
 	std::vector<std::string> texfilenames;
 	std::string name = filename.substr(0, filename.find_last_of("."));
 	texfilenames.push_back(name + ".1.ppm");
 
 	libcoco::ParserLuaFile parser;
 	parser.parser(filename.ToStdString());
 	parser.transToMemory(texfilenames);
 
 	std::vector<d2d::ISymbol*> symbols;
 	parser.getAllSymbols(symbols);
 	for (int i = 0, n = symbols.size(); i < n; ++i)
 		if (isHandleSymbol(symbols[i]))
 			m_list->insert(symbols[i]);
}

}