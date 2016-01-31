#include "LibraryPage.h"
#include "Symbol.h"
#include "config.h"

namespace etexture
{

LibraryPage::LibraryPage(wxWindow* parent)
	: ee::LibraryPage(parent, wxT("Texture"))
{
	InitLayout();
	m_list->SetFileter(FILE_TAG);
}

bool LibraryPage::IsHandleSymbol(ee::Symbol* symbol) const
{
	return dynamic_cast<Symbol*>(symbol) != NULL;
}

void LibraryPage::OnAddPress(wxCommandEvent& event)
{
	wxString filter = wxT("*_") + ee::FileType::GetTag(ee::FileType::e_texture) + wxT(".json");
	wxFileDialog dlg(this, wxT("导入texture文件"), wxEmptyString, wxEmptyString, filter, wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			std::string filename = filenames[i].ToStdString();
			try {
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