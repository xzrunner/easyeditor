#include "LibraryFontBlankPage.h"
#include "LibraryFontBlankList.h"
#include "FontBlankDialog.h"
#include "ExceptionDlg.h"

#include "dataset/SymbolMgr.h"
#include "dataset/FontBlankSymbol.h"
#include "common/FileNameParser.h"
#include "common/Exception.h"

#include <json/json.h>

namespace d2d
{

LibraryFontBlankPage::LibraryFontBlankPage(wxWindow* parent)
	: ILibraryPage(parent, wxT("FontBlank"))
{
	m_list = new LibraryFontBlankList(this);
	InitLayout();
}

bool LibraryFontBlankPage::IsHandleSymbol(ISymbol* symbol) const
{
	return dynamic_cast<FontBlankSymbol*>(symbol) != NULL;
}

bool LibraryFontBlankPage::LoadFromConfig()
{
	return ILibraryPage::LoadFromConfig("library_label");
}

void LibraryFontBlankPage::InitLayout(bool draggable /*= true*/)
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	{
		wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);

		wxButton* btnNew = new wxButton(this, wxID_ANY, "new", wxDefaultPosition, wxSize(40, 20));
		Connect(btnNew->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LibraryFontBlankPage::onNewBtnPress));
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
	wxString filter = FileNameParser::getFileTag(FileNameParser::e_fontblank);
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
				ISymbol* symbol = SymbolMgr::Instance()->FetchSymbol(filenames[i].ToStdString());
				m_list->insert(symbol);
				symbol->Release();
			} catch (Exception& e) {
				ExceptionDlg dlg(m_parent, e);
				dlg.ShowModal();
			}
		}
	}
}

void LibraryFontBlankPage::onNewBtnPress(wxCommandEvent& event)
{
	FontBlankSymbol* item = new FontBlankSymbol();
	m_list->insert(item);

	FontBlankDialog dlg(this, item);
	dlg.ShowModal();
}

} // d2d