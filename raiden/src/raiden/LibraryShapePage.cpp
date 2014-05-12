#include "LibraryShapePage.h"
#include "LibraryShapeList.h"
#include "Context.h"
#include "StagePanel.h"

#include <easyshape.h>

using namespace raiden;

LibraryShapePage::LibraryShapePage(wxWindow* parent)
	: d2d::ILibraryPage(parent, "Shape")
{
	m_list = new LibraryShapeList(this);
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
	wxFileDialog dlg(this, wxT("Select shapes"), wxEmptyString, 
		wxEmptyString, wxT("*_shape.json;"), wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filenames[i]);
			m_list->insert(symbol);
			symbol->release();
		}
	}
}

void LibraryShapePage::onNewBtnPress(wxCommandEvent& event)
{
	static int id = 0;
	libshape::Symbol* symbol = new libshape::Symbol;
	symbol->setName("shape"+wxString::FromDouble(id++));

	m_list->insert(symbol);

	{
		libshape::MainDialog<libshape::StagePanel> dlg(this, symbol);
		dlg.ShowModal();
	}
	Context::Instance()->stage->resetCanvas();
	symbol->refresh();
	Context::Instance()->library->Refresh();
	Context::Instance()->stage->resetPathOffset(symbol);
}