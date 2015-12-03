#include "LibraryScriptsPage.h"
#include "LibraryList.h"
#include "ExceptionDlg.h"

#include "dataset/ScriptsSymbol.h"
#include "dataset/SymbolMgr.h"
#include "common/FileNameParser.h"
#include "common/Exception.h"
#include "view/IStageCanvas.h"

namespace d2d
{

LibraryScriptsPage::LibraryScriptsPage(wxWindow* parent)
	: ILibraryPage(parent, wxT("Scripts"))
{
	InitLayout();
	m_list->SetFileter("scripts");
}

bool LibraryScriptsPage::IsHandleSymbol(d2d::ISymbol* symbol) const
{
	return dynamic_cast<ScriptsSymbol*>(symbol) != NULL;
}

bool LibraryScriptsPage::LoadFromConfig()
{
	return ILibraryPage::LoadFromConfig("library_script");
}

void LibraryScriptsPage::OnAddPress(wxCommandEvent& event)
{
	wxString filter = FileNameParser::getFileTag(FileNameParser::e_scripts);
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
				ISymbol* symbol = SymbolMgr::Instance()->FetchSymbol(filepath);
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