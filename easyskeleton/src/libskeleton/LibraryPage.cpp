#include "LibraryPage.h"
#include "Symbol.h"

#include <ee/LibraryList.h>
#include <ee/SymbolFile.h>
#include <ee/SymbolMgr.h>
#include <ee/Exception.h>
#include <ee/ExceptionDlg.h>
#include <ee/FileHelper.h>

#include <sprite2/SymType.h>

namespace libskeleton
{

LibraryPage::LibraryPage(wxWindow* parent)
	: ee::LibraryPage(parent, "Skeleton")
{
	InitLayout();
	m_list->SetFileter(ee::SymbolFile::Instance()->Tag(s2::SYM_SKELETON));
}

bool LibraryPage::IsHandleSymbol(ee::Symbol* sym) const
{
	return dynamic_cast<Symbol*>(sym) != NULL;
}

void LibraryPage::OnAddPress(wxCommandEvent& event)
{
	std::string ee_filter   = ee::FileHelper::GetJsonFileFilter(ee::SymbolFile::Instance()->Tag(s2::SYM_SKELETON)),
		        json_filter = "JSON files (*.json)|*.json";
	std::string filter = ee_filter + "|" + json_filter;
	wxFileDialog dlg(this, wxT("导入skeleton文件"), wxEmptyString, 
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
	ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(filename, s2::SYM_SKELETON);
//	sym->RefreshThumbnail(filename);
	m_list->Insert(sym);
	sym->RemoveReference();

	//Symbol* sym = new Symbol;
	//sym->LoadFromFile(filename);
	//std::string easy_filename = filename.substr(0, filename.find_last_of('.')) 
	//	+ "_" + ee::SymbolFile::Instance()->Tag(s2::SYM_SKELETON) + ".json";
	//sym->SetFilepath(easy_filename);
	//sym->RefreshThumbnail(filename);
	//m_list->Insert(sym);
	//sym->RemoveReference();
}

}