#include "LibraryPage.h"
#include "Symbol.h"

#include <ee/LibraryList.h>
#include <ee/SymbolFile.h>
#include <ee/SymbolMgr.h>
#include <ee/Exception.h>
#include <ee/ExceptionDlg.h>

namespace libanis
{

LibraryPage::LibraryPage(wxWindow* parent)
	: ee::LibraryPage(parent, "Anis")
{
	InitLayout();
	m_list->SetFileter(ee::SymbolFile::Instance()->Tag(ee::SYM_ANIS));
}

bool LibraryPage::IsHandleSymbol(ee::Symbol* sym) const
{
	return dynamic_cast<Symbol*>(sym) != NULL;
}

void LibraryPage::OnAddPress(wxCommandEvent& event)
{
	wxString filter = ee::SymbolFile::Instance()->Tag(s2::SYM_ANIMATION);
	wxFileDialog dlg(this, wxT("导入anis文件"), wxEmptyString, 
		wxEmptyString, filter, wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			try {
				std::string filename = filenames[i].ToStdString();
				ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(filename);
				sym->RefreshThumbnail(filename);
				m_list->Insert(sym);
				sym->RemoveReference();
			} catch (ee::Exception& e) {
				ee::ExceptionDlg dlg(m_parent, e);
				dlg.ShowModal();
			}
		}
	}
}

}