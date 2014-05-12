#include "LibraryComplexPage.h"
#include "LibraryComplexList.h"
#include "StagePanel.h"
#include "Context.h"

#include "complex/Dialog.h"

namespace coceditor
{

LibraryComplexPage::LibraryComplexPage(wxWindow* parent)
	: ILibraryPage(parent, wxT("Complex"))
{
	m_list = new LibraryComplexList(this);
	initLayout();
}

bool LibraryComplexPage::isHandleSymbol(d2d::ISymbol* symbol) const
{
	return dynamic_cast<complex::Symbol*>(symbol) != NULL;
}

void LibraryComplexPage::initLayout(bool draggable /*= true*/)
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	{
		wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);

		wxButton* btnNew = new wxButton(this, wxID_ANY, "new", wxDefaultPosition, wxSize(40, 20));
		Connect(btnNew->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LibraryComplexPage::onNewBtnPress));
		btnSizer->Add(btnNew, 0, wxRIGHT, 10);

		initButtons(btnSizer);

		sizer->Add(btnSizer, 0, wxALIGN_RIGHT);
	}
	{
		sizer->Add(m_list, 1, wxEXPAND);
	}
	SetSizer(sizer);
}

void LibraryComplexPage::onAddPress(wxCommandEvent& event)
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
 			complex::Symbol* item = new complex::Symbol;
 			item->loadFromFile(filenames[i]);
			item->refresh();
 			m_list->insert(item);
 		}
 	}
}

void LibraryComplexPage::onNewBtnPress(wxCommandEvent& event)
{
 	complex::Symbol* item = new complex::Symbol();
 	m_list->insert(item);
 
	complex::Dialog dlg(this, item, Context::Instance()->property);
 	dlg.ShowModal();
	Context::Instance()->stage->resetCanvas();
}
} // coceditor