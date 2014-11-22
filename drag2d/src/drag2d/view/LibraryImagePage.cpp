#include "LibraryImagePage.h"
#include "LibraryList.h"

#include "dataset/SymbolMgr.h"
#include "dataset/ImageSymbol.h"
#include "view/GLCanvas.h"

namespace d2d
{

LibraryImagePage::LibraryImagePage(wxWindow* parent, bool supportBmp/* = true*/)
	: ILibraryPage(parent, wxT("Image"))
	, m_supportBmp(supportBmp)
{
	initLayout();
}

bool LibraryImagePage::isHandleSymbol(ISymbol* symbol) const
{
	return dynamic_cast<ImageSymbol*>(symbol) != NULL;
}

void LibraryImagePage::onAddPress(wxCommandEvent& event)
{
	wxString formatFilter = m_supportBmp ? wxT("*.png;*.jpg;*.bmp") : wxT("*.png;*.jpg");
	wxFileDialog dlg(this, wxT("Choose images"), wxEmptyString, 
		wxEmptyString, formatFilter, wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			ISymbol* symbol = SymbolMgr::Instance()->fetchSymbol(filenames[i]);
			m_list->insert(symbol);
			symbol->Release();
		}
		if (m_canvas) {
			m_canvas->resetViewport();
		}
	}
}

} // d2d