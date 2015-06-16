#include "LibraryPage.h"
#include "Symbol.h"
#include "config.h"

#include <easycoco.h>

namespace ecomplex
{

LibraryPage::LibraryPage(wxWindow* parent)
	: d2d::ILibraryPage(parent, wxT("Complex"))
{
	InitLayout();
	m_list->setFileter(FILE_TAG);
}

bool LibraryPage::IsHandleSymbol(d2d::ISymbol* symbol) const
{
	return dynamic_cast<Symbol*>(symbol) != NULL;
}

void LibraryPage::LoadFromConfig()
{
	ILibraryPage::LoadFromConfig("library_complex");
}

void LibraryPage::OnAddPress(wxCommandEvent& event)
{
	wxString filter = d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_complex);
	filter = wxT("*_") + filter + wxT(".json");
	filter += wxT("; *.lua");
	wxFileDialog dlg(this, wxT("����complex�ļ�"), wxEmptyString, 
		wxEmptyString, filter, wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			const wxString& filename = filenames[i];
			std::string type = filename.substr(filename.find_last_of(".") + 1);
			StringTools::toLower(type);
			try {
				if (type == "json") {
					loadFromJsonFile(filename);
				} else if (type == "lua") {
					loadFromLuaFile(filename);
				}
			} catch (d2d::Exception& e) {
				d2d::ExceptionDlg dlg(m_parent, e);
				dlg.ShowModal();
			}
		}

		// rm resetinitstate
		if (m_canvas) {
			m_canvas->resetViewport();
		}
	}
}

void LibraryPage::loadFromJsonFile(const wxString& filename)
{
	d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filename);
	symbol->RefreshThumbnail(filename);
	m_list->insert(symbol);
	symbol->Release();
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
		if (IsHandleSymbol(symbols[i]))
			m_list->insert(symbols[i]);
}

}