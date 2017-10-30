#include "LibraryImagePage.h"
#include "LibraryList.h"
#include "ExceptionDlg.h"
#include "SymbolMgr.h"
#include "ImageSymbol.h"
#include "StageCanvas.h"
#include "Exception.h"
#include "SymbolType.h"

namespace ee
{

LibraryImagePage::LibraryImagePage(wxWindow* parent, bool supportBmp/* = true*/)
	: LibraryPage(parent, "Image")
	, m_support_bmp(supportBmp)
{
	InitLayout();
}

bool LibraryImagePage::IsHandleSymbol(const SymPtr& sym) const
{
	return sym->Type() == s2::SYM_IMAGE;
}

void LibraryImagePage::OnAddPress(wxCommandEvent& event)
{
	std::string formatFilter = m_support_bmp ? "*.png;*.jpg;*.bmp;*.pvr;*.pkm" : "*.png;*.jpg;*.bmp;*.pvr;*.pkm";
	wxFileDialog dlg(this, wxT("Choose images"), wxEmptyString, 
		wxEmptyString, formatFilter, wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			try {
				auto sym = SymbolMgr::Instance()->FetchSymbol(filenames[i].ToStdString());
				m_list->Insert(sym);
			} catch (Exception& e) {
				ExceptionDlg dlg(m_parent, e);
				dlg.ShowModal();
			}
		}
	}
}

}