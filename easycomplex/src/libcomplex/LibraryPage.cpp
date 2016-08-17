#include "LibraryPage.h"
#include "Symbol.h"
#include "config.h"

#include <ee/LibraryList.h>
#include <ee/FileType.h>
#include <ee/StringHelper.h>
#include <ee/ExceptionDlg.h>
#include <ee/Exception.h>
#include <ee/SymbolMgr.h>

#include <easycoco.h>

namespace ecomplex
{

LibraryPage::LibraryPage(wxWindow* parent)
	: ee::LibraryPage(parent, "Complex")
{
	InitLayout();
	m_list->SetFileter(FILE_TAG);
}

bool LibraryPage::IsHandleSymbol(ee::Symbol* sym) const
{
	return dynamic_cast<Symbol*>(sym) != NULL;
}

bool LibraryPage::LoadFromConfig()
{
	return ee::LibraryPage::LoadFromConfig("library_complex");
}

void LibraryPage::OnAddPress(wxCommandEvent& event)
{
	std::string tag = ee::FileType::GetTag(ee::FileType::e_complex);
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
			ee::StringHelper::ToLower(type);
			try {
				if (type == "json") {
					loadFromJsonFile(filename);
				} else if (type == "lua") {
					loadFromLuaFile(filename);
				}
			} catch (ee::Exception& e) {
				ee::ExceptionDlg dlg(m_parent, e);
				dlg.ShowModal();
			}
		}
	}
}

void LibraryPage::loadFromJsonFile(const std::string& filename)
{
	ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(filename);
	sym->RefreshThumbnail(filename);
	m_list->Insert(sym);
	sym->RemoveReference();
}

void LibraryPage::loadFromLuaFile(const std::string& filename)
{
// 	std::vector<std::string> texfilenames;
// 	std::string name = filename.substr(0, filename.find_last_of("."));
// 	texfilenames.push_back(name + ".1.ppm");
// 
// 	ecoco::epe::ParserLuaFile parser;
// 	parser.parser(filename);
// 	parser.transToMemory(texfilenames);
// 
// 	std::vector<ee::Symbol*> syms;
// 	parser.getAllSymbols(syms);
// 	for (int i = 0, n = syms.size(); i < n; ++i)
// 		if (IsHandleSymbol(syms[i]))
// 			m_list->Insert(syms[i]);
}

}