#include "LibraryScriptsPage.h"
#include "LibraryList.h"

#include "dataset/ScriptsSymbol.h"
#include "dataset/SymbolMgr.h"
#include "common/FileNameParser.h"
#include "view/GLCanvas.h"

namespace d2d
{
	LibraryScriptsPage::LibraryScriptsPage(wxWindow* parent)
		: ILibraryPage(parent, wxT("Scripts"))
	{
		initLayout();
		m_list->setFileter("scripts");
	}

	bool LibraryScriptsPage::isHandleSymbol(d2d::ISymbol* symbol) const
	{
		return dynamic_cast<ScriptsSymbol*>(symbol) != NULL;
	}

	void LibraryScriptsPage::onAddPress(wxCommandEvent& event)
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
				const wxString& filepath = filenames[i];
				ISymbol* symbol = SymbolMgr::Instance()->fetchSymbol(filepath);
				symbol->RefreshThumbnail(filepath);
				m_list->insert(symbol);
				symbol->Release();
			}

			if (m_canvas) {
				 m_canvas->resetViewport();
			}
		}
	}
}