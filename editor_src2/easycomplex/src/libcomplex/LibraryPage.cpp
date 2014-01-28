#include "LibraryPage.h"
#include "Symbol.h"

namespace complex
{
	LibraryPage::LibraryPage(wxWindow* parent)
		: d2d::ILibraryPage(parent, wxT("Complex"))
	{
		initLayout();
		m_list->setFileter("complex");
	}

	bool LibraryPage::isHandleSymbol(d2d::ISymbol* symbol) const
	{
		return dynamic_cast<Symbol*>(symbol) != NULL;
	}

	void LibraryPage::onAddPress(wxCommandEvent& event)
	{
		wxString filter = d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_complex);
		filter = wxT("*_") + filter + wxT(".json");
		wxFileDialog dlg(this, wxT("导入complex文件"), wxEmptyString, 
			wxEmptyString, filter, wxFD_OPEN | wxFD_MULTIPLE);
		if (dlg.ShowModal() == wxID_OK)
		{
			wxArrayString filenames;
			dlg.GetPaths(filenames);
			for (size_t i = 0, n = filenames.size(); i < n; ++i)
			{
				Symbol* item = new Symbol;
				item->loadFromFile(filenames[i]);
				item->refresh();
				m_list->insert(item);
			}
		}
	}
}