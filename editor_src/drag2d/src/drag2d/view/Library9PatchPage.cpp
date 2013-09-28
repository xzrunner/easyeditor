#include "Library9PatchPage.h"
#include "LibraryList.h"

#include "dataset/Patch9Symbol.h"
#include "common/FileNameParser.h"

namespace d2d
{

Library9PatchPage::Library9PatchPage(wxWindow* parent)
	: ILibraryPage(parent, wxT("9patch"))
{
	initLayout();
	m_list->setFileter("9patch");
}

bool Library9PatchPage::isHandleSymbol(d2d::ISymbol* symbol) const
{
	return dynamic_cast<Patch9Symbol*>(symbol) != NULL;
}

void Library9PatchPage::onAddPress(wxCommandEvent& event)
{
	wxString filter = FileNameParser::getFileTag(FileNameParser::e_9patch);
	filter = wxT("*_") + filter + wxT(".json");
	wxFileDialog dlg(this, wxT("导入9patch文件"), wxEmptyString, 
		wxEmptyString, filter, wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			Patch9Symbol* item = new Patch9Symbol;
			item->loadFromFile(filenames[i]);
			item->refresh();
			m_list->insert(item);
		}
	}
}

} // d2d