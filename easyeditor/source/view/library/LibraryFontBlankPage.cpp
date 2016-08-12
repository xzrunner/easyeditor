#include "LibraryFontBlankPage.h"
#include "LibraryFontBlankList.h"
#include "FontBlankDialog.h"
#include "ExceptionDlg.h"
#include "SymbolMgr.h"
#include "FontBlankSymbol.h"
#include "FileType.h"
#include "Exception.h"

#include <json/json.h>

namespace ee
{

LibraryFontBlankPage::LibraryFontBlankPage(wxWindow* parent)
	: LibraryPage(parent, "FontBlank")
{
	m_list = new LibraryFontBlankList(this);
	InitLayout();
}

bool LibraryFontBlankPage::IsHandleSymbol(Symbol* symbol) const
{
	return dynamic_cast<FontBlankSymbol*>(symbol) != NULL;
}

bool LibraryFontBlankPage::LoadFromConfig()
{
	return LibraryPage::LoadFromConfig("library_label");
}

void LibraryFontBlankPage::InitLayout(bool draggable /*= true*/)
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	{
		wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);

		wxButton* btnNew = new wxButton(this, wxID_ANY, "new", wxDefaultPosition, wxSize(40, 20));
		Connect(btnNew->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LibraryFontBlankPage::OnNewBtnPress));
		btnSizer->Add(btnNew, 0, wxRIGHT, 10);

		InitButtons(btnSizer);

		sizer->Add(btnSizer, 0, wxALIGN_RIGHT);
	}
	{
		sizer->Add(m_list, 1, wxEXPAND);
	}
	SetSizer(sizer);
}

void LibraryFontBlankPage::OnAddPress(wxCommandEvent& event)
{
	std::string filter = FileType::GetTag(FileType::e_fontblank);
	filter = wxT("*_") + filter + wxT(".json");
	wxFileDialog dlg(this, wxT("导入font blank文件"), wxEmptyString, 
		wxEmptyString, filter, wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			try {
				Symbol* symbol = SymbolMgr::Instance()->FetchSymbol(filenames[i].ToStdString());
				m_list->Insert(symbol);
				symbol->RemoveReference();
			} catch (Exception& e) {
				ExceptionDlg dlg(m_parent, e);
				dlg.ShowModal();
			}
		}
	}
}

void LibraryFontBlankPage::OnNewBtnPress(wxCommandEvent& event)
{
	FontBlankSymbol* item = new FontBlankSymbol();
	m_list->Insert(item);

	FontBlankDialog dlg(this, item);
	dlg.ShowModal();
}

}