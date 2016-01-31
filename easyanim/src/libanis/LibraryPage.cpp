#include "LibraryPage.h"
#include "Symbol.h"

namespace libanis
{

LibraryPage::LibraryPage(wxWindow* parent)
	: ee::LibraryPage(parent, wxT("Anis"))
{
	InitLayout();
	m_list->SetFileter(ee::FileType::GetTag(ee::FileType::e_anis).ToStdString());
}

bool LibraryPage::IsHandleSymbol(ee::Symbol* symbol) const
{
	return dynamic_cast<Symbol*>(symbol) != NULL;
}

void LibraryPage::OnAddPress(wxCommandEvent& event)
{
	wxString filter = ee::FileType::GetTag(ee::FileType::e_anim);
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
				ee::Symbol* symbol = ee::SymbolMgr::Instance()->FetchSymbol(filename);
				symbol->RefreshThumbnail(filename);
				m_list->Insert(symbol);
				symbol->Release();
			} catch (ee::Exception& e) {
				ee::ExceptionDlg dlg(m_parent, e);
				dlg.ShowModal();
			}
		}
	}
}

}