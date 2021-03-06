#include "LibraryPage.h"
#include "Symbol.h"

#include <ee/LibraryList.h>
#include <ee/SymbolFile.h>
#include <ee/SymbolMgr.h>
#include <ee/Exception.h>
#include <ee/ExceptionDlg.h>
#include <ee/SymbolType.h>

namespace eshadow
{

LibraryPage::LibraryPage(wxWindow* parent)
	: ee::LibraryPage(parent, "Shadow")
{
	InitLayout();
	m_list->SetFileter(ee::SymbolFile::Instance()->Tag(ee::SYM_SHADOW));
}

bool LibraryPage::IsHandleSymbol(const ee::SymPtr& sym) const
{
	return sym->Type() == ee::SYM_SHADOW;
}

void LibraryPage::OnAddPress(wxCommandEvent& event)
{
	wxString filter = ee::SymbolFile::Instance()->Tag(ee::SYM_SHADOW);
	filter = wxT("*_") + filter + wxT(".json");
	wxFileDialog dlg(this, wxT("����shadow�ļ�"), wxEmptyString, 
		wxEmptyString, filter, wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			std::string filepath = filenames[i].ToStdString();
			try {
				auto sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
				sym->RefreshThumbnail(filepath);
				AddItem(sym);
			} catch (ee::Exception& e) {
				ee::ExceptionDlg dlg(m_parent, e);
				dlg.ShowModal();
			}
		}
	}
}

}