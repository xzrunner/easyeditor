#include "LibraryImagePage.h"
#include "LibraryItem.h"
#include "StagePanel.h"
#include "ToolBarPanel.h"

#include <easyshape.h>

namespace eshape
{

LibraryImagePage::LibraryImagePage(wxWindow* parent)
	: LibraryPage(parent, "Image")
{
	initLayout();
}

bool LibraryImagePage::isHandleSymbol(d2d::ISymbol* symbol) const
{
	wxString ext = d2d::FilenameTools::getExtension(symbol->getFilepath()).Lower();
	return ext == "png" || ext == "jpg" || ext == "bmp";
}

void LibraryImagePage::onAddPress(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("Select images"), wxEmptyString, 
		wxEmptyString, wxT("*.png;*.jpg;"), wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			LibraryItem* item = new LibraryItem(filenames[i]);

			std::vector<d2d::IShape*>* shapes = new std::vector<d2d::IShape*>;
			item->setUserData(shapes);

			wxString shape_filepath = d2d::FilenameTools::getFilePathExceptExtension(filenames[i]) 
				+ "_" + d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_shape) + ".json";
			libshape::FileAdapter adapter(*shapes);
			adapter.load(shape_filepath);

			m_stage->changeCurrItem(item);
			m_toolbar->changeCurrItem(item);

			m_list->insert(item);
		}
	}
}

}