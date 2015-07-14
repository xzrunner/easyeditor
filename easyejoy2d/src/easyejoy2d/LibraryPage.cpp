#include "LibraryPage.h"
#include "Symbol.h"
#include "config.h"

namespace eejoy2d
{

LibraryPage::LibraryPage(wxWindow* parent)
	: d2d::ILibraryPage(parent, wxT("Ejoy2D"))
{
	InitLayout();
	m_list->SetFileter(FILE_TAG);
}

bool LibraryPage::IsHandleSymbol(d2d::ISymbol* symbol) const
{
	return dynamic_cast<Symbol*>(symbol) != NULL;
}

void LibraryPage::OnAddPress(wxCommandEvent& event)
{
	wxString filter = d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_ejoy2d);
	filter = wxT("*_") + filter + wxT(".json");
	wxFileDialog dlg(this, wxT("����Ejoy2D�ļ�"), wxEmptyString, 
		wxEmptyString, filter, wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			std::string filepath = filenames[i].ToStdString();
			try {
				d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(filepath);
				symbol->RefreshThumbnail(filepath);
				m_list->Insert(symbol);
				symbol->Release();
			} catch (d2d::Exception& e) {
				d2d::ExceptionDlg dlg(m_parent, e);
				dlg.ShowModal();
			}
		}

		if (!filenames.empty() && m_canvas) {
			m_canvas->ResetViewport();
		}
	}
}

}