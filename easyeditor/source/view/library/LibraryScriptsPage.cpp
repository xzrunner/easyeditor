#include "LibraryScriptsPage.h"
#include "LibraryList.h"
#include "ExceptionDlg.h"
#include "ScriptsSymbol.h"
#include "SymbolMgr.h"
#include "FileType.h"
#include "Exception.h"
#include "StageCanvas.h"

namespace ee
{

LibraryScriptsPage::LibraryScriptsPage(wxWindow* parent)
	: LibraryPage(parent, "Scripts")
{
	InitLayout();
	m_list->SetFileter("scripts");
}

bool LibraryScriptsPage::IsHandleSymbol(Symbol* symbol) const
{
	return dynamic_cast<ScriptsSymbol*>(symbol) != NULL;
}

bool LibraryScriptsPage::LoadFromConfig()
{
	return LibraryPage::LoadFromConfig("library_script");
}

void LibraryScriptsPage::OnAddPress(wxCommandEvent& event)
{
	std::string filter = FileType::GetTag(FileType::e_scripts);
	filter = wxT("*_") + filter + wxT(".lua");
	wxFileDialog dlg(this, wxT("导入scripts文件"), wxEmptyString, 
		wxEmptyString, filter, wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			try {
				std::string filepath = filenames[i].ToStdString();
				Symbol* symbol = SymbolMgr::Instance()->FetchSymbol(filepath);
				symbol->RefreshThumbnail(filepath);
				m_list->Insert(symbol);
				symbol->Release();
			} catch (Exception& e) {
				ExceptionDlg dlg(m_parent, e);
				dlg.ShowModal();
			}
		}
	}
}

}