#include "LibraryPage.h"
#include "config.h"
#include "Symbol.h"

namespace eshadow
{

LibraryPage::LibraryPage(wxWindow* parent)
	: d2d::ILibraryPage(parent, wxT("Shadow"))
{
	initLayout();
	m_list->setFileter(FILE_TAG);
}

bool LibraryPage::isHandleSymbol(d2d::ISymbol* symbol) const
{
	return dynamic_cast<Symbol*>(symbol) != NULL;
}

void LibraryPage::onAddPress(wxCommandEvent& event)
{
	wxString filter = d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_shadow);
	filter = wxT("*_") + filter + wxT(".json");
	wxFileDialog dlg(this, wxT("导入shadow文件"), wxEmptyString, 
		wxEmptyString, filter, wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			const wxString& filepath = filenames[i];
			try {
				d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filepath);
				symbol->RefreshThumbnail(filepath);
				AddItem(symbol);
				symbol->Release();
			} catch (d2d::Exception& e) {
				d2d::ExceptionDlg dlg(m_parent, e);
				dlg.ShowModal();
			}
		}

		// rm resetinitstate
		if (m_canvas) {
			m_canvas->resetViewport();
		}
	}
}

}