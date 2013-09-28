#include "LibraryComplexPage.h"
#include "LibraryList.h"

#include "dataset/ComplexSymbol.h"
#include "common/FileNameParser.h"

namespace d2d
{

LibraryComplexPage::LibraryComplexPage(wxWindow* parent)
	: ILibraryPage(parent, wxT("Complex"))
{
	initLayout();
	m_list->setFileter("complex");
}

bool LibraryComplexPage::isHandleSymbol(ISymbol* symbol) const
{
	return dynamic_cast<ComplexSymbol*>(symbol) != NULL;
}

void LibraryComplexPage::onAddPress(wxCommandEvent& event)
{
	wxString filter = FileNameParser::getFileTag(FileNameParser::e_complex);
	filter = wxT("*_") + filter + wxT(".json");
 	wxFileDialog dlg(this, wxT("导入complex文件"), wxEmptyString, 
 		wxEmptyString, filter, wxFD_OPEN | wxFD_MULTIPLE);
 	if (dlg.ShowModal() == wxID_OK)
 	{
 		wxArrayString filenames;
 		dlg.GetPaths(filenames);
 		for (size_t i = 0, n = filenames.size(); i < n; ++i)
 		{
 			ComplexSymbol* item = new ComplexSymbol;
 			item->loadFromFile(filenames[i]);
			item->refresh();
 			m_list->insert(item);
 		}
 	}
}
} // d2d