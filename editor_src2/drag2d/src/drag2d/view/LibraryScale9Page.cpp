#include "LibraryScale9Page.h"
#include "LibraryList.h"

#include "dataset/Scale9Symbol.h"
#include "common/FileNameParser.h"

namespace d2d
{

LibraryScale9Page::LibraryScale9Page(wxWindow* parent)
	: ILibraryPage(parent, wxT("Scale9"))
{
	initLayout();
	m_list->setFileter("scale9");
}

bool LibraryScale9Page::isHandleSymbol(d2d::ISymbol* symbol) const
{
	return dynamic_cast<Scale9Symbol*>(symbol) != NULL;
}

void LibraryScale9Page::onAddPress(wxCommandEvent& event)
{
	wxString filter = FileNameParser::getFileTag(FileNameParser::e_scale9);
	filter = wxT("*_") + filter + wxT(".json");
	wxFileDialog dlg(this, wxT("导入scale9文件"), wxEmptyString, 
		wxEmptyString, filter, wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			Scale9Symbol* item = new Scale9Symbol;
			item->loadFromFile(filenames[i]);
			item->refresh();
			m_list->insert(item);
		}
	}
}

} // d2d