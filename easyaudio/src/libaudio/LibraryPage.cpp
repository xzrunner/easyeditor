#include "LibraryPage.h"
#include "Symbol.h"

#include <ee/LibraryList.h>
#include <ee/SymbolFile.h>
#include <ee/SymbolMgr.h>
#include <ee/Exception.h>
#include <ee/ExceptionDlg.h>

#include <sprite2/SymType.h>

namespace eaudio
{

LibraryPage::LibraryPage(wxWindow* parent)
	: ee::LibraryPage(parent, "Audio")
{
	InitLayout();
	m_list->SetFileter("mp3");
}

bool LibraryPage::IsHandleSymbol(ee::Symbol* sym) const
{
	return sym->Type() == s2::SYM_AUDIO;
}

void LibraryPage::OnAddPress(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("导入audio文件"), wxEmptyString, 
		wxEmptyString, "*.mp3;*_complex.json", wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			std::string filepath = filenames[i];
			try {
				ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath, s2::SYM_AUDIO);
				m_list->Insert(sym);
				sym->RemoveReference();
			} catch (ee::Exception& e) {
				ee::ExceptionDlg dlg(m_parent, e);
				dlg.ShowModal();
			}
		}
	}
}

}