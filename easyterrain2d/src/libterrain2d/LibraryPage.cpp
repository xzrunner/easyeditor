#include "LibraryPage.h"
#include "Symbol.h"
#include "config.h"

#include <ee/LibraryList.h>
#include <ee/SymbolFile.h>
#include <ee/SymbolMgr.h>
#include <ee/Exception.h>
#include <ee/ExceptionDlg.h>
#include <ee/SymbolType.h>

namespace eterrain2d
{

LibraryPage::LibraryPage(wxWindow* parent)
	: ee::LibraryPage(parent, "Terrain2D")
{
	InitLayout();
	m_list->SetFileter(ee::SymbolFile::Instance()->Tag(ee::SYM_TERRAIN2D));
}

bool LibraryPage::IsHandleSymbol(ee::Symbol* sym) const
{
	return dynamic_cast<Symbol*>(sym) != NULL;
}

void LibraryPage::OnAddPress(wxCommandEvent& event)
{
	std::string filter = ee::SymbolFile::Instance()->Tag(ee::SYM_TERRAIN2D);
	filter = wxT("*_") + filter + wxT(".json");
	wxFileDialog dlg(this, wxT("导入terrain2d文件"), wxEmptyString, 
		wxEmptyString, filter, wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			std::string filepath = filenames[i].ToStdString();
			try {
				ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
				sym->RefreshThumbnail(filepath);
				AddItem(sym);
				sym->RemoveReference();
			} catch (ee::Exception& e) {
				ee::ExceptionDlg dlg(m_parent, e);
				dlg.ShowModal();
			}
		}
	}
}

}