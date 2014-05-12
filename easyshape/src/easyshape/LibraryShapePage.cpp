#include "LibraryShapePage.h"
#include "LibraryItem.h"
#include "StagePanel.h"
#include "ToolBarPanel.h"

#include <easyshape.h>

namespace eshape
{

LibraryShapePage::LibraryShapePage(wxWindow* parent)
	: LibraryPage(parent, "Shape")
{
	initLayout();
}

bool LibraryShapePage::isHandleSymbol(d2d::ISymbol* symbol) const
{
	const wxString& filepath = symbol->getFilepath();
	wxString ext = d2d::FilenameTools::getExtension(filepath).Lower();
	return ext == "json" && d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_shape);
}

void LibraryShapePage::initLayout(bool draggable /*= true*/)
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	{
		wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);

		wxButton* btnNew = new wxButton(this, wxID_ANY, "new", wxDefaultPosition, wxSize(40, 20));
		Connect(btnNew->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LibraryShapePage::onNewBtnPress));
		btnSizer->Add(btnNew, 0, wxRIGHT, 10);

		initButtons(btnSizer);

		sizer->Add(btnSizer, 0, wxALIGN_RIGHT);
	}
	{
		sizer->Add(m_list, 1, wxEXPAND);
	}
	SetSizer(sizer);
}

void LibraryShapePage::onAddPress(wxCommandEvent& event)
{
	wxString filter = "*_" + d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_shape) + ".json";
	wxFileDialog dlg(this, wxT("Select shapes"), wxEmptyString, 
		wxEmptyString, filter, wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			LibraryItem* item = new LibraryItem(filenames[i]);

			std::vector<d2d::IShape*>* shapes = new std::vector<d2d::IShape*>;
			item->setUserData(shapes);

			libshape::FileAdapter adapter(*shapes);
			adapter.load(filenames[i]);

			m_stage->changeCurrItem(item);
			m_toolbar->changeCurrItem(item);

			m_list->insert(item);
		}
	}
}

void LibraryShapePage::onNewBtnPress(wxCommandEvent& event)
{
	LibraryItem* item = new LibraryItem;

	std::vector<d2d::IShape*>* shapes = new std::vector<d2d::IShape*>;
	item->setUserData(shapes);

	m_stage->changeCurrItem(item);
	m_toolbar->changeCurrItem(item);

	m_list->insert(item);
}

}