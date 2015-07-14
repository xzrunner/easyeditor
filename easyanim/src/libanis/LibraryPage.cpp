#include "LibraryPage.h"
#include "Symbol.h"

namespace libanis
{

LibraryPage::LibraryPage(wxWindow* parent)
	: d2d::ILibraryPage(parent, wxT("Anis"))
{
	InitLayout();
	m_list->SetFileter(d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_anis).ToStdString());
}

bool LibraryPage::IsHandleSymbol(d2d::ISymbol* symbol) const
{
	return dynamic_cast<Symbol*>(symbol) != NULL;
}

void LibraryPage::OnAddPress(wxCommandEvent& event)
{
	wxString filter = d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_anim);
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
				d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(filename);
				symbol->RefreshThumbnail(filename);
				m_list->Insert(symbol);
				symbol->Release();
			} catch (d2d::Exception& e) {
				d2d::ExceptionDlg dlg(m_parent, e);
				dlg.ShowModal();
			}
		}
	}
}

}