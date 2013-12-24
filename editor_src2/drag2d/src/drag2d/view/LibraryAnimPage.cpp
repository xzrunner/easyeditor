#include "LibraryAnimPage.h"
#include "LibraryList.h"

#include "dataset/AnimSymbol.h"
#include "common/FileNameParser.h"

namespace d2d
{

LibraryAnimPage::LibraryAnimPage(wxWindow* parent)
	: ILibraryPage(parent, wxT("Anim"))
{
	initLayout();
	m_list->setFileter("anim");
}

bool LibraryAnimPage::isHandleSymbol(d2d::ISymbol* symbol) const
{
	return dynamic_cast<AnimSymbol*>(symbol) != NULL;
}

void LibraryAnimPage::onAddPress(wxCommandEvent& event)
{
	wxString filter = FileNameParser::getFileTag(FileNameParser::e_anim);
	filter = wxT("*_") + filter + wxT(".json");
	wxFileDialog dlg(this, wxT("导入anim文件"), wxEmptyString, 
		wxEmptyString, filter, wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			AnimSymbol* item = new AnimSymbol;
			item->loadFromFile(filenames[i]);
			item->refresh();
			m_list->insert(item);
		}
	}
}

} // d2d