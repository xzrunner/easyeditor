#include "LibraryPage.h"
#include "Symbol.h"

#include <ee/LibraryList.h>
#include <ee/SymbolFile.h>
#include <ee/SymbolMgr.h>
#include <ee/Exception.h>
#include <ee/ExceptionDlg.h>

#include <sprite2/SymType.h>

namespace eshape
{

LibraryPage::LibraryPage(wxWindow* parent)
	: ee::LibraryPage(parent, "Shape")
{
	InitLayout();
	m_list->SetFileter(ee::SymbolFile::Instance()->Tag(s2::SYM_SHAPE));
}

bool LibraryPage::IsHandleSymbol(const ee::SymPtr& sym) const
{
	return sym->Type() == s2::SYM_SHAPE;
}

void LibraryPage::OnAddPress(wxCommandEvent& event)
{
	std::string filter = "*_" + ee::SymbolFile::Instance()->Tag(s2::SYM_SHAPE) + ".json";
	wxFileDialog dlg(this, wxT("choose shape files"), wxEmptyString, 
		wxEmptyString, filter, wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			std::string filepath = filenames[i].ToStdString();
			try {
				auto sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
				sym->RefreshThumbnail(filepath);
				m_list->Insert(sym);
			} catch (ee::Exception& e) {
				ee::ExceptionDlg dlg(m_parent, e);
				dlg.ShowModal();
			}
		}
	}
}

}