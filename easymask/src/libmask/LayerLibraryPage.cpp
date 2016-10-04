#include "LayerLibraryPage.h"
#include "config.h"

#include <ee/LibraryList.h>
#include <ee/LibraryPanel.h>
#include <ee/SymbolMgr.h>
#include <ee/Symbol.h>
#include <ee/SymbolFile.h>

#include <sprite2/SymType.h>

namespace emask
{

LayerLibraryPage::LayerLibraryPage(wxWindow* parent, const std::string& name)
	: ee::LibraryPage(parent, name)
{
	InitLayout();
	m_list->SetFileter(ee::SymbolFile::Instance()->Tag(s2::SYM_MASK));
}

void LayerLibraryPage::OnAddPress(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("µº»Îsymbol"), wxEmptyString, 
		wxEmptyString, "*.*", wxFD_OPEN | wxFD_MULTIPLE);
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