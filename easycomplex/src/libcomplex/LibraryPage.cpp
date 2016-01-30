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
	m_list->SetFileter(FILE_TAG);
}

bool LibraryPage::IsHandleSymbol(d2d::Symbol* symbol) const
{
	return dynamic_cast<Symbol*>(symbol) != NULL;
}

bool LibraryPage::LoadFromConfig()
{
	return ILibraryPage::LoadFromConfig("library_complex");
}

void LibraryPage::OnAddPress(wxCommandEvent& event)
{
	std::string tag = d2d::FileType::GetTag(d2d::FileType::e_complex);
	std::string filter = "*_" + tag + ".json";
	filter += ";*_" + tag + "[gen].json";
	filter += "; *.lua";
	wxFileDialog dlg(this, wxT("导入complex文件"), wxEmptyString, 
		wxEmptyString, filter, wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			std::string filename = filenames[i].ToStdString();
			std::string type = filename.substr(filename.find_last_of(".") + 1);
			d2d::StringHelper::ToLower(type);
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
	}
}

void LibraryPage::loadFromJsonFile(const std::string& filename)
{
	d2d::Symbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(filename);
	symbol->RefreshThumbnail(filename);
	m_list->Insert(symbol);
	symbol->Release();
}

void LibraryPage::loadFromLuaFile(const std::string& filename)
{
// 	std::vector<std::string> texfilenames;
// 	std::string name = filename.substr(0, filename.find_last_of("."));
// 	texfilenames.push_back(name + ".1.ppm");
// 
// 	libcoco::epe::ParserLuaFile parser;
// 	parser.parser(filename);
// 	parser.transToMemory(texfilenames);
// 
// 	std::vector<d2d::Symbol*> symbols;
// 	parser.getAllSymbols(symbols);
// 	for (int i = 0, n = symbols.size(); i < n; ++i)
// 		if (IsHandleSymbol(symbols[i]))
// 			m_list->Insert(symbols[i]);
}

}