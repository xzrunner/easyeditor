#include "LibraryPage.h"

#include <ee/SymbolMgr.h>
#include <ee/LibraryList.h>
#include <ee/StringHelper.h>
#include <ee/SymbolFile.h>
#include <ee/SymbolType.h>

#include <easyui.h>
#include <easycomplex.h>

namespace eui
{
namespace window
{

LibraryPage::LibraryPage(wxWindow* parent, const char* name,
						 const std::string& filter)
	: ee::LibraryPage(parent, name)
	, m_filter(filter)
{
	InitLayout();
}

bool LibraryPage::IsHandleSymbol(ee::Symbol* sym) const
{
	if (m_filter == ee::SymbolFile::Instance()->Tag(ee::SYM_UIWND)) {
		return dynamic_cast<Symbol*>(sym) != NULL;	
	} else {
		return true;
	}
}

void LibraryPage::OnAddPress(wxCommandEvent& event)
{
	std::string filter;
	if (m_filter.empty()) {
		filter = "*.*";
	} else {
		filter = ee::StringHelper::Format("JSON files (*_%s.json)|*_%s.json", m_filter.c_str(), m_filter.c_str());
	}
	wxFileDialog dlg(this, wxT("µº»Îsymbol"), wxEmptyString, 
		wxEmptyString, filter, wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			std::string filepath = filenames[i].ToStdString();
			ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
			sym->RefreshThumbnail(filepath);
			m_list->Insert(sym);
			sym->RemoveReference();
		}
	}
}

}
}