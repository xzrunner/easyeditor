#include "LibraryPage.h"
#include "Symbol.h"

#include <ee/LibraryList.h>
#include <ee/SymbolFile.h>
#include <ee/SymbolMgr.h>
#include <ee/Exception.h>
#include <ee/ExceptionDlg.h>

#include <sprite2/SymType.h>

namespace emesh
{

LibraryPage::LibraryPage(wxWindow* parent)
	: ee::LibraryPage(parent, "Mesh")
{
	InitLayout();
	m_list->SetFileter(ee::SymbolFile::Instance()->Tag(s2::SYM_MESH));
}

bool LibraryPage::IsHandleSymbol(ee::Symbol* sym) const
{
	return dynamic_cast<Symbol*>(sym) != NULL;
}

void LibraryPage::OnAddPress(wxCommandEvent& event)
{
	std::string filter = ee::SymbolFile::Instance()->Tag(s2::SYM_MESH);
	filter = wxT("*_") + filter + wxT(".json");
	wxFileDialog dlg(this, wxT("导入mesh文件"), wxEmptyString, 
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