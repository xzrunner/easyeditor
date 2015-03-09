#include "LibraryPage.h"
#include "Symbol.h"
#include "config.h"

namespace etexture
{

LibraryPage::LibraryPage(wxWindow* parent)
	: d2d::ILibraryPage(parent, wxT("Texture"))
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
	wxString filter = wxT("*_") + d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_texture) + wxT(".json");
	wxFileDialog dlg(this, wxT("导入texture文件"), wxEmptyString, wxEmptyString, filter, wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			const wxString& filename = filenames[i];
			d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filename);
			symbol->RefreshThumbnail(filename);
			m_list->insert(symbol);
			symbol->Release();
		}

		if (m_canvas) {
			m_canvas->resetViewport();
		}
	}
}

}