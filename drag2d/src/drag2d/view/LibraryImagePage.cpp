#include "LibraryImagePage.h"
#include "LibraryList.h"
#include "ExceptionDlg.h"

#include "dataset/SymbolMgr.h"
#include "dataset/ImageSymbol.h"
#include "view/IStageCanvas.h"
#include "common/Exception.h"
#include "message/ResetViewportSJ.h"

namespace d2d
{

LibraryImagePage::LibraryImagePage(wxWindow* parent, bool supportBmp/* = true*/)
	: ILibraryPage(parent, wxT("Image"))
	, m_supportBmp(supportBmp)
{
	InitLayout();
}

bool LibraryImagePage::IsHandleSymbol(ISymbol* symbol) const
{
	return dynamic_cast<ImageSymbol*>(symbol) != NULL;
}

void LibraryImagePage::OnAddPress(wxCommandEvent& event)
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
			try {
				ISymbol* symbol = SymbolMgr::Instance()->FetchSymbol(filenames[i].ToStdString());
				m_list->Insert(symbol);
				symbol->Release();
			} catch (Exception& e) {
				ExceptionDlg dlg(m_parent, e);
				dlg.ShowModal();
			}
		}
	}
}

} // d2d