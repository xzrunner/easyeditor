#include "LibraryPage.h"
#include "Symbol.h"

#include <ee/LibraryList.h>
#include <ee/SymbolFile.h>
#include <ee/Exception.h>
#include <ee/ExceptionDlg.h>
#include <ee/StringHelper.h>
#include <ee/SymbolMgr.h>
#include <ee/FileHelper.h>

#include <easycomplex.h>
#include <easycoco.h>

#include <sprite2/SymType.h>

namespace libanim
{

LibraryPage::LibraryPage(wxWindow* parent)
	: ee::LibraryPage(parent, "Anim")
{
	InitLayout();
	m_list->SetFileter(ee::SymbolFile::Instance()->Tag(s2::SYM_ANIMATION));
}

bool LibraryPage::IsHandleSymbol(ee::Symbol* sym) const
{
	return dynamic_cast<Symbol*>(sym) != NULL;
}

void LibraryPage::OnAddPress(wxCommandEvent& event)
{
	std::string ee_filter = ee::FileHelper::GetJsonFileFilter(ee::SymbolFile::Instance()->Tag(s2::SYM_ANIMATION)),
		        json_filter = "JSON files (*.json)|*.json",
				lua_filter = "LUA files (*.lua)|*.lua";
	std::string filter = ee_filter + "|" + json_filter + "|" + lua_filter;
	wxFileDialog dlg(this, wxT("导入anim文件"), wxEmptyString, 
		wxEmptyString, filter, wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			const std::string filename = filenames[i];
			int idx = dlg.GetFilterIndex();
			try {
				switch (idx)
				{
				case 0:
					LoadFromEasyFile(filename);
					break;
				case 1:
					LoadFromJsonFile(filename);
					break;
				case 2:
					LoadFromLuaFile(filename);
					break;
				}
			} catch (ee::Exception& e) {
				ee::ExceptionDlg dlg(m_parent, e);
				dlg.ShowModal();
			}
		}
	}
}

void LibraryPage::LoadFromEasyFile(const std::string& filename)
{
	ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(filename);
	sym->RefreshThumbnail(filename);
	m_list->Insert(sym);
	sym->RemoveReference();
}

void LibraryPage::LoadFromJsonFile(const std::string& filename)
{
	Symbol* sym = new Symbol;
	sym->LoadFromFile(filename);
	std::string easy_filename = filename.substr(0, filename.find_last_of('.')) 
		+ "_" + ee::SymbolFile::Instance()->Tag(s2::SYM_ANIMATION) + ".json";
	sym->SetFilepath(easy_filename);
	m_list->Insert(sym);
	sym->RemoveReference();
}

void LibraryPage::LoadFromLuaFile(const std::string& filename)
{
 	std::vector<std::string> texfilenames;
 	std::string name = filename.substr(0, filename.find_last_of("."));
 	texfilenames.push_back(name + ".1.ppm");
 
	ecoco::epe::ParserLuaFile parser;
 	parser.parser(filename);
 	parser.transToMemory(texfilenames);
 
 	std::vector<ee::Symbol*> syms;
 	parser.getAllSymbols(syms);
 	for (int i = 0, n = syms.size(); i < n; ++i)
 		if (IsHandleSymbol(syms[i]))
 			m_list->Insert(syms[i]);
}

}